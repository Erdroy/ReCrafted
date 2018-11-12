// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/10/2018 13:30:01 Source: 'EmptyActor.API.cpp' Target: 'Common/EmptyActor.Gen.cs'

using ReCrafted.API.Common;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	EmptyActor class.
	/// </summary>
	public partial class EmptyActor : ActorBase
	{
        internal EmptyActor() {}

        /// <summary>
        /// Creates new actor of type 'EmptyActor'.
        /// </summary>
        /// <returns>The newly created actor.</returns>
	    public static EmptyActor Create()
	    {
	        return New<EmptyActor>();
	    }
	}
}
