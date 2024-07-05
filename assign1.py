
class CacheSimulator:
    def __init__(self, cache_size, block_size, main_memory_size):
        self.cache_size = cache_size
        self.block_size = block_size
        self.main_memory_size = main_memory_size
        self.num_blocks = cache_size // block_size
        self.cache = [-1] * self.num_blocks  # Initialize cache with -1 to indicate empty block
        self.hits = 0
        self.misses = 0
        self.evictions = 0
    
    def read(self, address):
        block_number = address // self.block_size
        tag = block_number // self.num_blocks
        index = block_number % self.num_blocks
        if self.cache[index] == tag: #checks if the tag stored in the cache index matches the tag given
            self.hits += 1
            result = "Hit"
        else:
            self.misses += 1
            if self.cache[index] != -1: #if cache index is not empty
                self.evictions += 1
            self.cache[index] = tag
            result = "Miss"
        print(f"Read: Address {address}, Result: {result}, Cache: {self.cache}")
        return result
    
    def write(self, address): #write through 
        block_number = address // self.block_size
        tag = block_number // self.num_blocks
        index = block_number % self.num_blocks
        if self.cache[index] == tag:
            #self.hits += 1
            result = "Hit"
        else:
            # self.misses += 1
            # if self.cache[index] != -1:
            #     self.evictions += 1
            self.cache[index] = tag
            result = "Miss"
        # Write through to main memory regardless of hit or miss
        main_memory_block = tag * self.num_blocks + index
        print(f"Write through to main memory: Block {main_memory_block}")
        print(f"Write: Address {address}, Result: {result}, Cache: {self.cache}")
        return result

# Input memory addresses
def input_addresses():
    addresses = []
    while True:
        try:
            address = int(input("Enter memory address (or -1 to stop): "))
            if address == -1:
                break
            addresses.append(address)
        except ValueError:
            print("Invalid input. Please enter a valid memory address.")
    return addresses

# Cache configuration
cache_size = 64
block_size = 8
main_memory_size = 4096
cache_simulator = CacheSimulator(cache_size, block_size, main_memory_size)

# Accept memory addresses from user
addresses = input_addresses()

# Simulate cache behavior for each memory address
for address in addresses:
    cache_simulator.read(address)
    cache_simulator.write(address)

# Display final cache statistics
print(f"Total Hits: {cache_simulator.hits}, Misses: {cache_simulator.misses}, Evictions: {cache_simulator.evictions}")
