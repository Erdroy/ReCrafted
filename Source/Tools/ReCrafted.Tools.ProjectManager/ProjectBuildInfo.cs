// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections.Generic;
using ReCrafted.Tools.Common;

namespace ReCrafted.Tools.ProjectManager
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
