using System;

namespace Mono.Debugging.VisualStudio
{
	public static class Guids
	{
		public readonly static Guid guidFilterRegisters;

		public readonly static Guid guidFilterLocals;

		public readonly static Guid guidFilterAllLocals;

		public readonly static Guid guidFilterArgs;

		public readonly static Guid guidFilterLocalsPlusArgs;

		public readonly static Guid guidFilterAllLocalsPlusArgs;

		public readonly static Guid guidLanguageCpp;

		public readonly static Guid guidLanguageCSharp;

		public const string EngineGuidString = "9E1626AE-7DB7-4138-AC41-641D55CF9A4A";

		public readonly static Guid EngineGuid;

		public const string EngineClassGuidString = "C094C059-1786-49CD-8EB9-9C0EF6CA5454";

		public readonly static Guid EngineClassGuid;

		public const string PortSupplierGuidString = "FCEE747D-E866-4221-BD96-99C30E4B67B9";

		public readonly static Guid PortSupplierGuid;

		public const string PortSupplierClassGuidString = "A2C0CC70-C265-4807-901D-2E5A6378BF43";

		public readonly static Guid PortSupplierClassGuid;

		public const string PackageGuidString = "2315269F-53E7-4844-8213-8F954654B744";

		public const string ProgramPublisher = "d04d550d-1ea8-4e37-830e-700fea447688";

		static Guids()
		{
			Guids.guidFilterRegisters = new Guid("223ae797-bd09-4f28-8241-2763bdc5f713");
			Guids.guidFilterLocals = new Guid("b200f725-e725-4c53-b36a-1ec27aef12ef");
			Guids.guidFilterAllLocals = new Guid("196db21f-5f22-45a9-b5a3-32cddb30db06");
			Guids.guidFilterArgs = new Guid("804bccea-0475-4ae7-8a46-1862688ab863");
			Guids.guidFilterLocalsPlusArgs = new Guid("e74721bb-10c0-40f5-807f-920d37f95419");
			Guids.guidFilterAllLocalsPlusArgs = new Guid("939729a8-4cb0-4647-9831-7ff465240d5f");
			Guids.guidLanguageCpp = new Guid("3a12d0b7-c26c-11d0-b442-00a0244a1dd2");
			Guids.guidLanguageCSharp = new Guid("449ec4cc-30d2-4032-9256-ee18eb41b62b");
			Guids.EngineGuid = new Guid("9E1626AE-7DB7-4138-AC41-641D55CF9A4A");
			Guids.EngineClassGuid = new Guid("C094C059-1786-49CD-8EB9-9C0EF6CA5454");
			Guids.PortSupplierGuid = new Guid("FCEE747D-E866-4221-BD96-99C30E4B67B9");
			Guids.PortSupplierClassGuid = new Guid("A2C0CC70-C265-4807-901D-2E5A6378BF43");
		}
	}
}