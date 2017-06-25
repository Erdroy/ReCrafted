// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System.Collections.Generic;

namespace ReCrafted.APIGen.Tags
{
    public class APITagFile : APITag
    {
        public string SourceFileName { get; set; }

        public string TargetFileName { get; set; }

        public List<APITagUsing> Usings = new List<APITagUsing>();

        public List<APITagClass> Classes = new List<APITagClass>();

        //public List<APITagEnum> Enums = new List<APITagEnum>();
        //public List<APITagStruct> Structs = new List<APITagStruct>();
        //public List<APITagDelegate> Delegates = new List<APITagDelegate>();
    }
}
