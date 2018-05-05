// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace ReCrafted.ProjectManager
{
    // source: https://stackoverflow.com/questions/5411694/get-all-inherited-classes-of-an-abstract-class
    public static class ReflectiveEnumerator
    {
        public static IEnumerable<T> GetEnumerableOfType<T>(params object[] constructorArgs) where T : class
        {
            var objects = Assembly.GetAssembly(typeof(T))
                .GetTypes()
                .Where(myType => myType.IsClass && !myType.IsAbstract && myType.IsSubclassOf(typeof(T)))
                .Select(type => (T) Activator.CreateInstance(type, constructorArgs))
                .ToList();
            objects.Sort();
            return objects;
        }
    }
}
