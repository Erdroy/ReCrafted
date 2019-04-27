// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

namespace ReCrafted.Tools.APIGenerator.Description
{
    public struct TypeDescription
    {
        public string BaseType { get; set; }
        public bool IsConst { get; set; }
        public bool ByRef { get; set; }
        public bool ByPtr { get; set; }

        public override string ToString()
        {
            return $"{(IsConst ? "const " : "")}{BaseType}{(ByRef ? "&" : "")}{(ByPtr ? "*" : "")}";
        }
    }
}
