// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCrafted.LocaleConverter
{
    internal struct MinJson
    {
        private string _data;

        public void Add(string name, string value)
        {
            _data += "\"" + name + "\" : \"" + value + "\",";
        }

        public void Add(string name, int value)
        {
            _data += "\"" + name + "\" : " + value + ",";
        }

        public void Add(string name, ulong value)
        {
            _data += "\"" + name + "\" : " + value + ",";
        }

        public void AddManual(string name, string value)
        {
            _data += "\"" + name + "\" : " + value + ",";
        }

        public string Text
        {
            get
            {
                if (string.IsNullOrEmpty(_data))
                    return "{}";

                var temp = _data;

                temp = temp.Insert(0, "{\n"); // add '{' before text
                temp = temp.Remove(temp.Length - 1); // remove last ','
                temp += "\n}"; // add '}' after text

                return temp;
            }
        }
    }
}
