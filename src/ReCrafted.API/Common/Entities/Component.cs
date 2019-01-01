// ReCrafted (c) 2016-2019 Always Too Late

namespace ReCrafted.API.Common.Entities
{
    /// <summary>
    /// ECS component interface.
    /// </summary>
    public interface IComponent
    {
        ushort ComponentTypeId { get; }
        bool IsNativeComponent { get; }
        uint SizeInBytes { get; }
    }

    /// <summary>
    /// ECS component descriptor interface.
    /// </summary>
    public interface IComponentDescriptor : IComponent { }

    /// <summary>
    /// Empty ECS component data structure.
    /// Used for getting native component data pointer in EntityInternals.
    /// </summary>
    public struct ComponentData { }
}
