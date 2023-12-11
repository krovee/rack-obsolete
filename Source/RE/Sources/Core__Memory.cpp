#include <RE/Core/Memory.h>

#include <string.h>
#include <stdlib.h>

namespace re::low_level {
	void* memset(void* block, std::int8_t byte, size_t const& bytes) noexcept {
		return ::memset(block, byte, bytes);
	}
	void* memcopy(void* block, const void* source, size_t const& bytes) noexcept {
		return ::memcpy(block, source, bytes);
	}
	int memcompare(const void* blocka, const void* blockb, size_t const& bytes) noexcept {
		return ::memcmp(blocka, blockb, bytes);
	}
}

namespace re::low_level {
	static constexpr auto get_tag_index(allocation_tag tag) noexcept {
		return static_cast<std::int16_t>(tag);
	}
	memory_manager::memory_manager() noexcept {
		// TODO(krovee): make a specific string allocator??
		auto& basic_alloc = providers[get_tag_index(allocation_tag::other)];
		auto& string_alloc = providers[get_tag_index(allocation_tag::string)];
		string_alloc = new fixed_heap_memory_provider(268'435'456); // 256mb
		basic_alloc = new fixed_heap_memory_provider(536'870'912); // 512mb
		memory_tracker.records[get_tag_index(allocation_tag::other)].reserved = 536'870'912;
		memory_tracker.records[get_tag_index(allocation_tag::string)].reserved = 268'435'456;
	}
	memory_manager::~memory_manager() noexcept {
		delete reinterpret_cast<fixed_heap_memory_provider*>(providers[get_tag_index(allocation_tag::other)]);
		delete reinterpret_cast<fixed_heap_memory_provider*>(providers[get_tag_index(allocation_tag::string)]);
	}
	char* memory_manager::allocate_string(size_t const& length) noexcept {
		auto& string_alloc = providers[get_tag_index(allocation_tag::string)];
		char* pstr = reinterpret_cast<char*>(string_alloc->allocate(length + 1));
		re::low_level::memset(pstr, 0, length + 1);
		on_allocate(allocation_tag::string, length);
		return pstr;
	}
	void memory_manager::deallocate_string(char* str) noexcept {
		auto& string_alloc = providers[get_tag_index(allocation_tag::string)];
		const auto size = fixed_heap_memory_provider::get_block_size(str);
		on_deallocate(allocation_tag::string, size);
		string_alloc->deallocate(str);
	}
	void* memory_manager::allocate(size_t const& length) noexcept {
		auto& basic_alloc = providers[get_tag_index(allocation_tag::other)];
		on_allocate(allocation_tag::other, length);
		return basic_alloc->allocate(length);
	}
	void memory_manager::deallocate(void* block) noexcept {
		auto& basic_alloc = providers[get_tag_index(allocation_tag::other)];
		const auto size = fixed_heap_memory_provider::get_block_size(block);
		on_deallocate(allocation_tag::other, size);
		basic_alloc->deallocate(block);
	}
	void memory_manager::on_allocate(allocation_tag tag, size_t const& bytes) noexcept {
		auto& rec = memory_tracker.records[get_tag_index(tag)];
		rec.current_allocated += bytes;
		if (rec.current_allocated > rec.max_allocated) {
			rec.max_allocated = rec.current_allocated;
		}
	}
	void memory_manager::on_deallocate(allocation_tag tag, size_t const& bytes) noexcept {
		auto& rec = memory_tracker.records[get_tag_index(tag)];
		rec.current_allocated -= bytes;
	}

	fixed_heap_memory_provider::fixed_heap_memory_provider(size_t const& initial_capacity) noexcept
		: capacity(align_by(initial_capacity)) { // capacity is aligned by default
		
		// allocate enough space for both useful heap and free_blocks
		auto whole_heap = malloc(capacity + INITIAL_FREE_BLOCK_SIZE + 32);
		track_blocks_count = INITIAL_FREE_BLOCKS_COUNT;
		if (nullptr == whole_heap) {
			abort();
		}
		// get usable heap memory pointer
		memory = reinterpret_cast<intptr_t>(whole_heap);
		// get free blocks memory pointer
		track_blocks = reinterpret_cast<heap_block*>(memory + 32 /* small gap */ + capacity);
		// initialize free blocks with zeros
		memset(track_blocks, 0, INITIAL_FREE_BLOCK_SIZE);
	}
	fixed_heap_memory_provider::~fixed_heap_memory_provider() noexcept {
		if (0 != memory) {
			free(reinterpret_cast<void*>(memory));
		}
	}
	void* fixed_heap_memory_provider::allocate(size_t const& bytes) {
		// firstly, check edge cases..
		if (0 == bytes || capacity <= bytes) {
			return nullptr;
		}

		// so, get first free block which is fits
		auto blk = find_free_block(bytes);
		if (nullptr == blk) {
			blk = expand_track_blocks();
		}

		if (blk->size != 0 && blk->size >= bytes) {
			// so, reuse block
			return reinterpret_cast<void*>(blk->address + sizeof(block_header));
		}

		// get heap raw memory pointer
		auto raw_pointer = memory + memory_ptr;
		memory_ptr += bytes + sizeof(block_header);
		blk->address = raw_pointer;
		blk->size = bytes;
		blk->used = true;

		auto hdr = reinterpret_cast<block_header*>(raw_pointer);
		hdr->block = blk;

		return reinterpret_cast<void*>(raw_pointer + sizeof(block_header));
	}
	void* fixed_heap_memory_provider::reallocate(void* initial_mem_block, size_t const& bytes) {
		// so, idk how to make real reallocate function
		void* new_block = allocate(bytes);
		memcopy(new_block, initial_mem_block, bytes);
		deallocate(initial_mem_block);
		return new_block;
	}
	void fixed_heap_memory_provider::deallocate(void* mem_block) noexcept {
		const auto mem_block_pointer = reinterpret_cast<intptr_t>(mem_block);
		const auto hdr = reinterpret_cast<block_header*>(mem_block_pointer - sizeof(block_header));
		const auto blk = hdr->block;
		blk->used = false;
	}
	size_t fixed_heap_memory_provider::get_block_size(void* mem_block) noexcept {
		const auto mem_block_pointer = reinterpret_cast<intptr_t>(mem_block);
		const auto hdr = reinterpret_cast<block_header*>(mem_block_pointer - sizeof(block_header));
		const auto blk = hdr->block;
		return blk->size;
	}
	fixed_heap_memory_provider::heap_block* 
	fixed_heap_memory_provider::find_free_block(size_t const& required_size) noexcept  {
		for (size_t i = 0; i < track_blocks_count; ++i) {
			const auto blk = track_blocks + i;
			if (false == blk->used) {
				if (blk->size == 0 || blk->size >= required_size) {
					return track_blocks + i;
				}
			}
		}
		return nullptr;
	}
	fixed_heap_memory_provider::heap_block*
	fixed_heap_memory_provider::expand_track_blocks() noexcept {
		// what we need is to realloc whole heap with new count to extend current heap
		const auto new_free_blocks_array = track_blocks + track_blocks_count;
		const auto free_blocks_size = (track_blocks_count * sizeof(heap_block));
		const auto whole_heap_size = capacity + free_blocks_size;
		const auto new_whole_heap_size = whole_heap_size + free_blocks_size;
		track_blocks_count *= 2; // double blocks count

		auto whole_heap = realloc(reinterpret_cast<void*>(memory), new_whole_heap_size);
		memory = reinterpret_cast<intptr_t>(whole_heap);
		track_blocks = reinterpret_cast<heap_block*>(memory + capacity);
		memset(reinterpret_cast<void*>(memory + capacity + free_blocks_size), 0, free_blocks_size);
		return track_blocks + (track_blocks_count / 2);
	}
}
