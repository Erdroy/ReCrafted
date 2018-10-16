// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.API.Common.Entities
{
    public interface IComponentDescriptor
    {
        ushort ComponentId { get; }
        bool IsNativeComponent { get; }
    }
}
