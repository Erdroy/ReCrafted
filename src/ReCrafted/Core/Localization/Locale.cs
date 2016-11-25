// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using System.IO;
using Newtonsoft.Json;
using ReCrafted.Utilities;

namespace ReCrafted.Core.Localization
{
    public static class Locale
    {
        /// <summary>
        /// The current language.
        /// </summary>
        public static string Language { get; private set; }

        /// <summary>
        /// The current locale strings.
        /// </summary>
        public static LocaleStrings Strings;

        /// <summary>
        /// Gets all languages which are supported.
        /// </summary>
        /// <returns>Languages</returns>
        public static string[] GetLanguages()
        {
            throw new NotImplementedException(); // TODO: Find all available languages
        }
        
        /// <summary>
        /// Sets language.
        /// </summary>
        /// <param name="language">The language, must be the same as locale file name.</param>
        public static void SetLanguage(string language)
        {
            var lang = language.ToLower();
            var fileName = "./assets/locales/" + lang + ".json";
            
            if (!File.Exists(fileName))
            {
                throw new ReCraftedException("Language not found!");
            }
            
            // load
            var contents = File.ReadAllText(fileName);
            Strings = JsonConvert.DeserializeObject<LocaleStrings>(contents);
            
            Language = lang;
        }
    }
}