#include <cstddef>

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
    
} // namespace day00

