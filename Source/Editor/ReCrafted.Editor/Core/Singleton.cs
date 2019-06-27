// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Core
{
    public class Singleton<T> where T : class, new()
    {
        private static T _instance;
        public static T Instance => _instance ?? (_instance = new T());
    }
}
