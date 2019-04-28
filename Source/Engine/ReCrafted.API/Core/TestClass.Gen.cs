// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
    public sealed class TestClass : Object
    {

        public int Destroy(int testParam)
        {
            return InternalDestroy(NativePtr, testParam);
        }

        public void TestFunction()
        {
            InternalTestFunction(NativePtr);
        }

        public void TestFunction1(Vector3 vector)
        {
            InternalTestFunction1(NativePtr, vector);
        }

        public void TestFunction2(TestClass obj)
        {
            InternalTestFunction2(NativePtr, obj);
        }

        public void TestFunction3(ref Vector3 vector, Vector3 vector1)
        {
            InternalTestFunction3(NativePtr, ref vector, vector1);
        }

        public static TestClass Create()
        {
            return InternalCreate();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int InternalDestroy(System.IntPtr nativePtr, int testParam);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalTestFunction(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalTestFunction1(System.IntPtr nativePtr, Vector3 vector);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalTestFunction2(System.IntPtr nativePtr, TestClass obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalTestFunction3(System.IntPtr nativePtr, ref Vector3 vector, Vector3 vector1);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern TestClass InternalCreate();
    }
}