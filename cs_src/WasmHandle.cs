using System;
using System.IO;
using System.Collections.Generic;
using Wasmtime;

namespace WasmTest
{
    /// <summary>
    /// Handles loading and interacting with WASM modules using Wasmtime
    /// </summary>
    public class WasmHandle : IDisposable
    {
        private readonly Engine _engine;
        private readonly Store _store;
        private readonly Instance _instance;
        private readonly Dictionary<string, Function> _functions;
        private bool _disposed = false;

        public Store Store => _store;
        public Instance Instance => _instance;

        public WasmHandle(string wasmPath)
        {
            if (!File.Exists(wasmPath))
            {
                throw new FileNotFoundException($"WASM file not found: {wasmPath}");
            }

            Console.WriteLine($"Loading WASM module from: {wasmPath}");

            // Create engine and store
            _engine = new Engine();
            _store = new Store(_engine);

            // Load the module
            var moduleBytes = File.ReadAllBytes(wasmPath);
            var module = Module.FromBytes(_engine, "mod", moduleBytes);

            // Create the memory growth notification function
            // Emscripten expects this import for memory management
            var notifyMemoryGrowth = Function.FromCallback(_store, (int memoryIndex) => { });

            // Create instance with imports
            _instance = new Instance(_store, module, notifyMemoryGrowth);

            // Cache functions
            _functions = new Dictionary<string, Function>();
            
            Console.WriteLine("Module loaded successfully");
        }

        /// <summary>
        /// Get a function export by name
        /// </summary>
        public Function GetFunction(string name)
        {
            if (_functions.TryGetValue(name, out var cachedFunction))
            {
                return cachedFunction;
            }

            try 
            {
                var function = _instance.GetFunction(name);
                if (function == null)
                {
                    throw new InvalidOperationException($"Function '{name}' not found in WASM exports");
                }
                
                _functions[name] = function;
                return function;
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException($"Failed to get function '{name}': {ex.Message}", ex);
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed && disposing)
            {
                _store?.Dispose();
                _engine?.Dispose();
                _disposed = true;
            }
        }
    }
}