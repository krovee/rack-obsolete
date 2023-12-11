/*
* File:		RE/Core/Memory.h
* Author:	@krovee
* Date:		12/10/23
*
* Desc:		
*/
#pragma once

#include <RE/Core/Exports.h>
#include <cstdint>

namespace re {
	constexpr size_t align_by(size_t const& n, size_t const& align = sizeof(size_t)) {
		return (n + align - 1) & ~(align - 1);
	}
}

namespace re::low_level {
	void*	memset(void* block, std::int8_t byte, size_t const& bytes) noexcept;
	void*	memcopy(void* block, const void* source, size_t const& bytes) noexcept;
	int		memcompare(const void* blocka, const void* blockb, size_t const& bytes) noexcept;

	/*
	* enum class AllocationTag {};
	* Memory in Rack Engine is all controlled manually
	* and tracked by AllocationTag's.
	*/
	enum class allocation_tag : std::uint16_t {
		/*
		* allocation_tag::other (0)
		* Allocation with no tag goes into 'other' tag.
		* This is pretty much should be largest chunk
		* of memory.
		*/
		other,
		/*
		* allocation_tag::string (1)
		*/
		string,

		_max_count // used for count amount of tags
	};

	static constexpr auto ALLOCATION_TAG_COUNT = 
		static_cast<size_t>(allocation_tag::_max_count);

	class memory_manager;

	/*
	* 
	*/
	class memory_allocations_tracker {
	public:
		/*
		* 
		*/
		struct record {
			size_t current_allocated = {};
			size_t max_allocated = {};
			size_t reserved = {};
		};

		constexpr auto get_record(allocation_tag tag) const noexcept {
			return records[static_cast<std::uint16_t>(tag)];
		}

	private:
		record records[ALLOCATION_TAG_COUNT] = {};

		friend class memory_manager;
	};

	/*
	* global memory_allocations_tracker memory_tracker;
	*/
	static memory_allocations_tracker memory_tracker = memory_allocations_tracker();

	/*
	* class re::low_level::basic_memory_provider {};
	* Base class for every heap and stack memory providers,
	* which are used to allocate memory all the time.
	*/
	class basic_memory_provider {
	public:
		virtual ~basic_memory_provider() noexcept = default;

		/*
		* basic_memory_provider::allocate(bytes)
		*/
		virtual void*	allocate(size_t const& bytes) { return nullptr; }
		/*
		* basic_memory_provider::reallocate(initial_mem_block, bytes)
		*/
		virtual void*	reallocate(void* initial_mem_block, size_t const& bytes) { return nullptr; }
		/*
		* basic_memory_provider::allocate(mem_block)
		*/
		virtual void	deallocate(void* mem_block) noexcept {}
	};

	/*
	*/
	class memory_manager {
	public:
		memory_manager() noexcept;
		virtual ~memory_manager() noexcept;

		char* allocate_string(size_t const& length) noexcept;
		void deallocate_string(char* str) noexcept;

		void* allocate(size_t const& length) noexcept;
		void deallocate(void* block) noexcept;

	private:
		static void on_allocate(allocation_tag tag, size_t const& bytes) noexcept;
		static void on_deallocate(allocation_tag tag, size_t const& bytes) noexcept;

		basic_memory_provider* providers[ALLOCATION_TAG_COUNT] = {};
	};

	/*
	* class re::low_level::stack_memory_provider<N> {};
	* Stack-based memory provider with fast (but small size)
	* memory management.
	*/
	template<size_t BYTES_CAPACITY>
	class stack_memory_provider final : public basic_memory_provider {
	public:
		/*
		* stack_memory_provider::allocate(bytes)
		*/
		void*	allocate(size_t const& bytes) override;
		/*
		* stack_memory_provider::reallocate(initial_mem_block, bytes)
		*/
		void*	reallocate(void* initial_mem_block, size_t const& bytes) override;
		/*
		* stack_memory_provider::deallocate(mem_block)
		*/
		void	deallocate(void* mem_block) noexcept override;

		static size_t get_block_size(void* mem_block) noexcept;
	private:
		struct allocation_header {
			size_t size = {};
		};

		static inline const size_t raw_capacity = BYTES_CAPACITY + (sizeof(allocation_header) * (BYTES_CAPACITY / 8));
		static inline constexpr auto get_real_allocation_size(size_t const& requested_size) noexcept {
			return sizeof(allocation_header) + requested_size;
		}

		std::int8_t	memory[raw_capacity] = {};
		size_t		allocated = {};
	};

	/*
	* class fixed_heap_memory_provider {}; 
	* Memory provider for any-size memory allocations, but in
	* certain memory area. (any size, fixed size allocator)
	*/
	class fixed_heap_memory_provider final : public basic_memory_provider {
	public:
		fixed_heap_memory_provider(size_t const& initial_capacity) noexcept;
		virtual ~fixed_heap_memory_provider() noexcept;

		/*
		* fixed_heap_memory_provider::allocate(bytes)
		*/
		void*	allocate(size_t const& bytes) override;
		/*
		* fixed_heap_memory_provider::reallocate(initial_mem_block, bytes)
		*/
		void*	reallocate(void* initial_mem_block, size_t const& bytes) override;
		/*
		* fixed_heap_memory_provider::deallocate(mem_block)
		*/
		void	deallocate(void* mem_block) noexcept override;

		static size_t get_block_size(void* mem_block) noexcept;
	private:
		struct heap_block {
			intptr_t address = {};
			size_t size = {};
			bool used = false;
		};

		struct block_header {
			heap_block* block;
		};

		enum {
			INITIAL_FREE_BLOCKS_COUNT = 1024,
			INITIAL_FREE_BLOCK_SIZE = align_by(INITIAL_FREE_BLOCKS_COUNT * sizeof(heap_block)),
		};

		/*
		*		  (in-memory presentation)
		* [ ... useful fixed heap ... | track blocks... ] 
		*/
		heap_block* find_free_block(size_t const& required_size) noexcept;
		heap_block* expand_track_blocks() noexcept;

		heap_block*	 track_blocks = nullptr;
		size_t		 track_blocks_count = {};
		intptr_t 	 memory = {};
		size_t		 memory_ptr = {};
		size_t		 capacity = {};
	};

}

//
// Templates implementation
//

namespace re::low_level {
	template<size_t BYTES_CAPACITY>
	inline void* stack_memory_provider<BYTES_CAPACITY>::allocate(size_t const& bytes) {
		const auto real_bytes = get_real_allocation_size(bytes);
		const auto address = allocated + real_bytes;
		// in case of 0==bytes we pretty much OK, because in this case
		// function will return pointer to last allocated memory. (> mb a hack?)
		if (address >= BYTES_CAPACITY) {
			// ok, we're out of memory!
			return nullptr;
		}
		
		auto hdr = reinterpret_cast<allocation_header*>(memory + address);
		hdr->size = bytes;
		allocated = address;
		return reinterpret_cast<void*>(address + memory + sizeof(allocation_header));
	}
	template<size_t BYTES_CAPACITY>
	inline void* stack_memory_provider<BYTES_CAPACITY>::
	reallocate(void* initial_mem_block, size_t const& bytes) {
		// NOTE(krovee):	there's really no way to reallocate memory
		//					in stack-based memory, so all wa can do is
		//					to allocate brand-new memory block and copy
		//					initial data into that block, but ran into
		//					waste of memory because old block is unused.

		// so, firstly check parameters
		if (nullptr == initial_mem_block) {
			// ok, allocate new block immediately!
			return allocate(bytes);
		}

		// now, let's get initial memory block's address and it's header
		auto initial_bytes_pointer = reinterpret_cast<std::int8_t*>(initial_mem_block);
		auto hdr0 = reinterpret_cast<allocation_header*>(initial_bytes_pointer - sizeof(allocation_header));
		// ..and compare it to find out if this is our last allocation
		// so then we can just expand allocated space of that block with
		// no need to full-new allocation!
		if (initial_bytes_pointer == memory + allocated - hdr0->size) {
			// OK, just expand/reduce allocated memory, if possible!
			const int64_t to_expand = (int64_t)bytes - (int64_t)hdr0->size;
			allocated += to_expand; 
			// reflect new size.
			hdr0->size = bytes;
			// return same pointer
			return initial_bytes_pointer;
		}

		// so, bad news - there should be allocated new block.
		auto new_block = allocate(bytes);
		if (nullptr == new_block) {
			// if no block was provided and we're out of space:
			return nullptr;
		}

		// copy data into new block
		memcopy(new_block, initial_bytes_pointer, hdr0->size);
		// return new block.
		return new_block;
	}
	template<size_t BYTES_CAPACITY>
	void stack_memory_provider<BYTES_CAPACITY>::
	deallocate(void* mem_block) noexcept {
		// NOTE(krovee):	so, really there's no way to deallocate
		//					memory in stack-based approach.. BUT!
		//					if there's a situation where our mem_block
		//					is a last allocated memory block and
		//					we can move backward pointer in memory
		//					storage to reuse that area in next allocation.

		// test edge case
		if (nullptr == mem_block) {
			return;
		}

		// get memory block header
		auto mem_block_pointer = reinterpret_cast<std::int8_t*>(mem_block);
		auto hdr0 = reinterpret_cast<allocation_header*>(mem_block_pointer - sizeof(allocation_header));

		// check if this memory block is last allocated
		if (mem_block_pointer == memory + allocated - hdr0->size) {
			// OK, just reduce this::allocated.
			allocated -= hdr0->size;
		}

		// if memory block is not a last that was allocated,
		// we can't do anything.
	}
	template<size_t BYTES_CAPACITY>
	inline size_t stack_memory_provider<BYTES_CAPACITY>::get_block_size(void* mem_block) noexcept {
		// get memory block header
		auto mem_block_pointer = reinterpret_cast<std::int8_t*>(mem_block);
		auto hdr0 = reinterpret_cast<allocation_header*>(mem_block_pointer - sizeof(allocation_header));
		return hdr0->size;
	}
}