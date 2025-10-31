using System;
using Xunit;
using Xunit.Abstractions;

namespace WasmTest
{
    /// <summary>
    /// Test class for WASM functions using Wasmtime.
    /// Tests the exported functions: add, multiply, factorial, power
    /// </summary>
    public class WasmTimeTests : IDisposable
    {
        private readonly WasmHandle _wasmHandle;
        private readonly ITestOutputHelper _output;

        public WasmTimeTests(ITestOutputHelper output)
        {
            _output = output;
            _wasmHandle = new WasmHandle("mod.wasm");
        }

        [Fact]
        public void TestAddFunction()
        {
            _output.WriteLine("\n=== Testing add function ===");
            var addFunc = _wasmHandle.GetFunction("add");

            var testCases = new (int a, int b)[]
            {
                (5, 3), (10, -2), (0, 0), (-5, -10), (100, 200)
            };

            foreach (var (a, b) in testCases)
            {
                var result = (int)addFunc.Invoke(a, b)!;
                _output.WriteLine($"add({a}, {b}) = {result}");
                Assert.Equal(a + b, result);
            }

            _output.WriteLine("✓ add function tests passed!");
        }

        [Fact]
        public void TestMultiplyFunction()
        {
            _output.WriteLine("\n=== Testing multiply function ===");
            var multiplyFunc = _wasmHandle.GetFunction("multiply");

            var testCases = new (int a, int b)[]
            {
                (5, 3), (10, -2), (0, 5), (-5, -10), (7, 8)
            };

            foreach (var (a, b) in testCases)
            {
                var result = (int)multiplyFunc.Invoke(a, b)!;
                _output.WriteLine($"multiply({a}, {b}) = {result}");
                Assert.Equal(a * b, result);
            }

            _output.WriteLine("✓ multiply function tests passed!");
        }

        [Fact]
        public void TestFactorialFunction()
        {
            _output.WriteLine("\n=== Testing factorial function ===");
            var factorialFunc = _wasmHandle.GetFunction("factorial");

            var testCases = new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 };

            foreach (var n in testCases)
            {
                var result = (int)factorialFunc.Invoke(n)!;
                var expected = CalculateFactorial(n);
                _output.WriteLine($"factorial({n}) = {result}");
                Assert.Equal(expected, result);
            }

            _output.WriteLine("✓ factorial function tests passed!");
        }

        [Fact]
        public void TestPowerFunction()
        {
            _output.WriteLine("\n=== Testing power function ===");
            var powerFunc = _wasmHandle.GetFunction("power");

            var testCases = new (double baseValue, int exponent)[]
            {
                (2.0, 3),
                (5.0, 2),
                (10.0, 0),
                (3.14, 1),
                (2.5, 4),
                (-2.0, 3),
                (1.5, 5)
            };

            foreach (var (baseValue, exponent) in testCases)
            {
                var result = (double)powerFunc.Invoke(baseValue, exponent)!;
                var expected = Math.Pow(baseValue, exponent);
                _output.WriteLine($"power({baseValue}, {exponent}) = {result}");
                
                // Use approximate comparison for floating point
                Assert.True(Math.Abs(result - expected) < 1e-10,
                    $"Expected {expected}, got {result}");
            }

            _output.WriteLine("✓ power function tests passed!");
        }

        /// <summary>
        /// C# implementation of factorial for comparison
        /// </summary>
        private static int CalculateFactorial(int n)
        {
            if (n <= 1)
                return 1;
            
            int result = 1;
            for (int i = 2; i <= n; i++)
            {
                result *= i;
            }
            return result;
        }

        public void Dispose()
        {
            _wasmHandle?.Dispose();
        }
    }
}