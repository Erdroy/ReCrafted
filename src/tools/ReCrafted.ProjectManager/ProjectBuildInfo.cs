// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using ReCrafted.Common;

namespace ReCrafted.ProjectManager
{
    public class ProjectBuildInfo : JsonTemplate<ProjectBuildInfo>
    {
        public Dictionary<string, int> BuildCounts { get; set; }
        public DateTime LastBuild { get; set; }
        public string BuildName { get; set; }
    }
}
