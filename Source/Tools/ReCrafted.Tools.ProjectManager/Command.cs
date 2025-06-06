﻿// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.Tools.ProjectManager
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
