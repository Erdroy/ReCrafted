// ReCrafted (c) 2016-2018 Always Too Late

using System.IO;
using Newtonsoft.Json;

namespace ReCrafted.Common
{
    /// <summary>
    /// JsonTemplate base class, implements few useful functions for JSON (de) serialization.
    /// </summary>
    /// <typeparam name="T">The derived type/</typeparam>
    public class JsonTemplate<T> where T : class
    {
        /// <summary>
        /// Converts this object to JSON and then writes the JSON to given file.
        /// </summary>
        /// <param name="fileName">The file name.</param>
        /// <param name="formatting">The formatting type.</param>
        public void ToFile(string fileName, Formatting formatting = Formatting.None)
        {
            // Write to the file
            using (var file = new FileStream(fileName, FileMode.Create))
            {
                using (var writer = new StreamWriter(file))
                {
                    // Convert this object to JSON
                    var json = ToJson(formatting);

                    // Write the converted JSON to the file
                    writer.Write(json);
                }
            }
        }

        /// <summary>
        /// Converts this object to JSON.
        /// </summary>
        /// <param name="formatting">The formatting type.</param>
        /// <returns>The converted JSON string.</returns>
        public string ToJson(Formatting formatting = Formatting.None)
        {
            return JsonConvert.SerializeObject(this, formatting);
        }

        /// <summary>
        /// Reads JSON string from given file and then makes T object from it.
        /// </summary>
        /// <param name="fileName">The file name.</param>
        /// <returns>The created object or null when file is not found.</returns>
        public static T FromFile(string fileName)
        {
            // Read the file
            using (var file = new FileStream(fileName, FileMode.Open))
            {
                using (var reader = new StreamReader(file))
                {
                    // Read the file to the end
                    var json = reader.ReadToEnd();

                    // Convert from JSON
                    return FromJson(json);
                }
            }
        }

        /// <summary>
        /// Makes T object from given JSON string.
        /// </summary>
        /// <param name="json">The JSON string.</param>
        /// <returns>The created object.</returns>
        public static T FromJson(string json)
        {
            return JsonConvert.DeserializeObject<T>(json);
        }
    }
}
