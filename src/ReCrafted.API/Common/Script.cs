// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCrafted.API.Common
{
    public partial class Script
    {
        protected internal virtual void OnCreate() { }

        protected internal virtual void OnUpdate() { }

        protected internal virtual void OnPhysics() { }

        protected internal virtual void OnDestroy() { }
        
        public Entity Entity { get; internal set; }
    }
}
