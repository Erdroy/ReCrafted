// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using Newtonsoft.Json;
using System;
using System.IO;
using System.Linq;

namespace ReCrafted.Core.Locales
{
    public delegate void OnSetLocale();

    public class Locale
    {
        public static OnSetLocale OnSetLocale;

        private static object _currentLocale;

        public static void SetLocale(string language)
        {
            if (!GetLanguages().Contains(language))
            {
                throw new Exception(language + " language is not avaiable!");
            }

            var json = File.ReadAllText($"locale/{language}.json");
            _currentLocale = JsonConvert.DeserializeObject(json, new JsonSerializerSettings
            {
                Formatting = Formatting.Indented
            });

            OnSetLocale?.Invoke();
        }

        public static string[] GetLanguages()
        {
            var files = Directory.GetFiles("Locale", "*.json");

            return
                files.Select(file => new FileInfo(file))
                    .Select(fileInfo => fileInfo.Name.Replace(fileInfo.Extension, ""))
                    .ToArray();
        }

        public static string Resolve(string fieldName)
        {
            if (_currentLocale == null)
                return $"<missing-{fieldName}-locale>";

            var fields = _currentLocale.GetType().GetFields();

            foreach (var field in fields)
            {
                if (string.Equals(field.Name, fieldName, StringComparison.CurrentCultureIgnoreCase))
                {
                    return (string) field.GetValue(_currentLocale);
                }
            }

            return $"<missing-{fieldName}-locale>";
        }
    }
}