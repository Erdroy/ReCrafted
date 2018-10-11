// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.API.Common.Entities
{
    /// <summary>
    /// ECS component interface.
    /// </summary>
    public interface IComponent
    {
        ushort ComponentTypeId { get; }
        bool IsNativeComponent { get; }
    }
}
