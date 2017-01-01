// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System.IO;
using Newtonsoft.Json;

namespace ReCrafted.Core
{
    public class GameInfo
    {
        public static GameInfo Current;

        public string BuildName { get; set; }
        public int BuildNumber { get; set; }

        public static void Load()
        {
            var contents = File.ReadAllText("assets/gameinfo.json");
            Current = JsonConvert.DeserializeObject<GameInfo>(contents);
        }
    }
}