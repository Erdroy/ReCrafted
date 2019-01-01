// ReCrafted (c) 2016-2019 Always Too Late

using System;

namespace ReCrafted.ProjectManager
{
    public abstract class Command : IComparable<Command>
    {
        public int CompareTo(Command other)
        {
            return string.CompareOrdinal(Name, other.Name);
        }

        public abstract void Execute();

        public string Name => GetType().Name;
    }
}
