// ReCrafted © 2016-2017 Always Too Late

using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using ReCrafted.API.Core;

namespace ReCrafted.Core.Locales
{
    public delegate void OnSetLocale();

    public static class Locale
    {
        public static event OnSetLocale OnSetLocale;

        private static Dictionary<string, string> _currentLocale;

        /// <summary>
        /// Load locale of given language (if exists)
        /// </summary>
        /// <param name="language"></param>
        public static void SetLocale(string language)
        {
            if (!GetLanguages().Contains(language))
            {
                throw new Exception(language + " language is not avaliable!");
            }

            Logger.Log("Loading locale...");

            var json = File.ReadAllText($"../locales/{language}.json");
            var obj = JsonConvert.DeserializeObject<Dictionary<string, string>>(json);

            _currentLocale = obj;

            OnSetLocale?.Invoke();
        }

        /// <summary>
        /// Returns all avaiable languages
        /// </summary>
        /// <returns></returns>
        public static string[] GetLanguages()
        {
            var files = Directory.GetFiles("../locales", "*.json");

            return
                files.Select(file => new FileInfo(file))
                    .Select(fileInfo => fileInfo.Name.Replace(fileInfo.Extension, ""))
                    .ToArray();
        }

        /// <summary>
        /// Returns given locale's value from current language
        /// </summary>
        /// <param name="fieldName"></param>
        /// <returns></returns>
        public static string Resolve(string fieldName)
        {
            if (_currentLocale == null)
                return "<locales-not-loaded>";

            return _currentLocale.ContainsKey(fieldName) ? _currentLocale[fieldName] : $"<missing-{fieldName}-locale>";
        }
    }
}