// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.ProjectManager
{
    public abstract class Command
    {
        public abstract void Execute();
        
        public string Name => GetType().Name;
    }
}
