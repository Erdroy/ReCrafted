// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using ReCrafted.Common;

namespace ReCrafted.ProjectManager
{
    public class ProjectBuildInfo : JsonTemplate<ProjectBuildInfo>
    {
        public class UserBuildInfo
        {
            public int BuildCount { get; set; }
            public DateTime BuildDate { get; set; }
        }

        public Dictionary<string, UserBuildInfo> Info { get; set; }
        public string BuildName { get; set; }
    }
}
