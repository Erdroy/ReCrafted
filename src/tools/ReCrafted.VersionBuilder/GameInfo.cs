
using System.IO;
using Newtonsoft.Json;

namespace ReCrafted.Core
{
    public class GameInfo
    {
        public static GameInfo Current;

        public string BuildName { get; set; }
        public int BuildNumber { get; set; }

        public static void Generate(string name, string buildNum, string output)
        {
            Current = new GameInfo
            {
                BuildName = name,
                BuildNumber = int.Parse(File.ReadAllText(buildNum))
            };
            var gameinfo = JsonConvert.SerializeObject(Current, Formatting.Indented);
            File.WriteAllText(output, gameinfo);
        }
    }
}