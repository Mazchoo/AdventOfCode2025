#include <cstddef>
#include <cstdint>

#ifdef __EMSCRIPTEN__
#include <wasm_simd128.h>
#endif

namespace day00
{
    int add(int a, int b) {
        return a + b;
    }

    // Vector class for WASM bindings
    class FloatVector {
    private:
        float* data;
        int size;
        
    public:
        // Constructor
        FloatVector(int vec_size) : size(vec_size) {
            if (size > 0) {
                data = new float[size]();  // Initialize to zero
            } else {
                data = nullptr;
                size = 0;
            }
        }
        
        // Destructor
        ~FloatVector() {
            if (data != nullptr) {
                delete[] data;
            }
        }
        
        // Get the sum of all elements
        float sum() const {
            if (data == nullptr || size <= 0) {
                return 0.0f;
            }
            
            float total = 0.0f;
            for (int i = 0; i < size; i++) {
                total += data[i];
            }
            return total;
        }
        
        // Set element at index
        bool set_element(int index, float value) {
            if (data == nullptr || index < 0 || index >= size) {
                return false;
            }
            data[index] = value;
            return true;
        }
        
        // Get element at index
        float get_element(int index) const {
            if (data == nullptr || index < 0 || index >= size) {
                return 0.0f;
            }
            return data[index];
        }
        
        // Get size
        int get_size() const {
            return size;
        }
        
        // Get raw data pointer (for advanced use)
        float* get_data() {
            return data;
        }
    };

    // Factory function to create a new FloatVector
    FloatVector* create_float_vector(int size) {
        return new FloatVector(size);
    }
    
    // Function to free a FloatVector
    void free_float_vector(FloatVector* vector) {
        if (vector != nullptr) {
            delete vector;
        }
    }
    
    // Function to get sum from vector pointer
    float sum_float_vector(FloatVector* vector) {
        if (vector == nullptr) {
            return 0.0f;
        }
        return vector->sum();
    }
    
    // Function to set element via vector pointer
    bool set_vector_element(FloatVector* vector, int index, float value) {
        if (vector == nullptr) {
            return false;
        }
        return vector->set_element(index, value);
    }
    
    // Function to get element via vector pointer
    float get_vector_element(FloatVector* vector, int index) {
        if (vector == nullptr) {
            return 0.0f;
        }
        return vector->get_element(index);
    }
    
    // Raw pointer versions for performance comparison
    
    // Creates a raw float array of specified size, initialized to zero
    float* create_float_array(int size) {
        if (size <= 0) {
            return nullptr;
        }
        float* array = new float[size]();  // () initializes to zero
        return array;
    }
    
    // Frees memory allocated for a raw float array
    void free_float_array(float* ptr) {
        if (ptr != nullptr) {
            delete[] ptr;
        }
    }
    
    // Sums all elements in a raw float array
    float sum_float_array(float* ptr, int size) {
        if (ptr == nullptr || size <= 0) {
            return 0.0f;
        }
        
        float sum = 0.0f;
        for (int i = 0; i < size; i++) {
            sum += ptr[i];
        }
        return sum;
    }
    
    // Sets a value at specified index in raw float array
    bool set_array_element(float* ptr, int index, float value) {
        if (ptr == nullptr || index < 0) {
            return false;
        }
        ptr[index] = value;
        return true;
    }
    
    // Gets a value at specified index in raw float array
    float get_array_element(float* ptr, int index) {
        if (ptr == nullptr || index < 0) {
            return 0.0f;
        }
        return ptr[index];
    }
    
    // Byte array versions for uint8_t operations
    
    // Creates a raw uint8_t array of specified size, initialized to zero
    uint8_t* create_byte_array(int size) {
        if (size <= 0) {
            return nullptr;
        }
        uint8_t* array = new uint8_t[size]();  // () initializes to zero
        return array;
    }
    
    // Frees memory allocated for a raw uint8_t array
    void free_byte_array(uint8_t* ptr) {
        if (ptr != nullptr) {
            delete[] ptr;
        }
    }
 
    // Sums all elements in a raw uint8_t array (returns int to avoid overflow)
    int sum_byte_array(uint8_t* ptr, int size) {
        if (ptr == nullptr || size <= 0) {
            return 0;
        }

        int sum = 0;
        for (int i = 0; i < size; i++) {
            sum += ptr[i];
        }
        return sum;
    }

#ifdef __EMSCRIPTEN__
    // SIMD-optimized version for WASM: Process 16 bytes at a time using 128-bit SIMD
    int sum_byte_array_simd(uint8_t* ptr, int size) {
        if (ptr == nullptr || size <= 0) {
            return 0;
        }
        
        int sum = 0;
        int i = 0;
        
        // Process 16 bytes at a time using SIMD
        const int simd_block_size = 16;
        const int simd_blocks = size / simd_block_size;
        
        if (simd_blocks > 0) {
            // Initialize accumulator vector to zero
            v128_t sum_vec = wasm_i32x4_splat(0);
            
            for (int block = 0; block < simd_blocks; block++) {
                // Load 16 bytes into SIMD register
                v128_t bytes_vec = wasm_v128_load(&ptr[i]);
                
                // Process bytes in groups of 4 to fit in 32-bit lanes
                // Use pointer arithmetic to access individual bytes
                const uint8_t* byte_ptr = &ptr[i];
                
                // Sum groups of 4 bytes each
                uint32_t sum0 = byte_ptr[0] + byte_ptr[1] + byte_ptr[2] + byte_ptr[3];
                uint32_t sum1 = byte_ptr[4] + byte_ptr[5] + byte_ptr[6] + byte_ptr[7];
                uint32_t sum2 = byte_ptr[8] + byte_ptr[9] + byte_ptr[10] + byte_ptr[11];
                uint32_t sum3 = byte_ptr[12] + byte_ptr[13] + byte_ptr[14] + byte_ptr[15];
                
                // Create vector with the 4 sums and add to accumulator
                v128_t partial_sums = wasm_i32x4_make(sum0, sum1, sum2, sum3);
                sum_vec = wasm_i32x4_add(sum_vec, partial_sums);
                
                i += simd_block_size;
            }
            
            // Extract the 4 lanes using array access instead of extract_lane
            int32_t result[4];
            wasm_v128_store(result, sum_vec);
            sum += result[0] + result[1] + result[2] + result[3];
        }
        
        // Handle remaining bytes (if any)
        for (; i < size; i++) {
            sum += ptr[i];
        }
        
        return sum;
    }
#else
    // Default to regular implementation for C++ build
    int sum_byte_array_simd(uint8_t* ptr, int size) {
        return sum_byte_array(ptr, size);
    }
#endif
    
    // Sets a value at specified index in raw uint8_t array
    bool set_byte_array_element(uint8_t* ptr, int index, uint8_t value) {
        if (ptr == nullptr || index < 0) {
            return false;
        }
        ptr[index] = value;
        return true;
    }
    
    // Gets a value at specified index in raw uint8_t array
    uint8_t get_byte_array_element(uint8_t* ptr, int index) {
        if (ptr == nullptr || index < 0) {
            return 0;
        }
        return ptr[index];
    }
    
} // namespace day00

