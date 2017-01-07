// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

namespace ReCrafted.Graphics
{
    /// <summary>
    /// RasterizerStates enum.
    /// </summary>
    public enum RasterizerStates
    {
        Default = BackFaceCulled,
        FrontFaceCulled = 1,
        BackFaceCulled = 2,
        Wireframe = 3,
        WireframeBackFaceCulled = 4
    }
}