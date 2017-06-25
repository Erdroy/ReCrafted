// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCrafted.APIGen
{
    public abstract class APITag
    {
        public abstract void Process(string token, string[] parameters);
    }
}