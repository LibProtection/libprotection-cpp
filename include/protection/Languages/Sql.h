#ifndef PROTECTION_SQL_H
#define PROTECTION_SQL_H

#include "protection/AntlrLanguageProvider.h"
#include "protection/Token.h"

namespace protection {
namespace injections {

enum class SqlTokenType {
  Space = 1,
  CommentInput = 2,
  LineComment = 3,
  Dummy = 4,
  Abs = 5,
  Absolute = 6,
  Acos = 7,
  Action = 8,
  Add = 9,
  Adddate = 10,
  Addtime = 11,
  AesDecrypt = 12,
  AesEncrypt = 13,
  After = 14,
  Agent = 15,
  Aggregate = 16,
  Algorithm = 17,
  All = 18,
  Allowed = 19,
  AllowSnapshotIsolation = 20,
  Alter = 21,
  Analyze = 22,
  And = 23,
  AnsiNulls = 24,
  AnsiNullDefault = 25,
  AnsiPadding = 26,
  AnsiWarnings = 27,
  Any = 28,
  Apply = 29,
  Archive = 30,
  Area = 31,
  Arithabort = 32,
  Armscii8 = 33,
  Array = 34,
  As = 35,
  Asbinary = 36,
  Asc = 37,
  Ascii = 38,
  Asin = 39,
  Associate = 40,
  Astext = 41,
  Aswkb = 42,
  Aswkt = 43,
  AsymmetricDecrypt = 44,
  AsymmetricDerive = 45,
  AsymmetricEncrypt = 46,
  AsymmetricSign = 47,
  AsymmetricVerify = 48,
  At = 49,
  Atan = 50,
  Atan2 = 51,
  Attribute = 52,
  Audit = 53,
  Authid = 54,
  Authorization = 55,
  Authors = 56,
  Auto = 57,
  Autocommit = 58,
  AutoextendSize = 59,
  Automatic = 60,
  AutonomousTransaction = 61,
  AutoCleanup = 62,
  AutoClose = 63,
  AutoCreateStatistics = 64,
  AutoIncrement = 65,
  AutoShrink = 66,
  AutoUpdateStatistics = 67,
  AutoUpdateStatisticsAsync = 68,
  Avg = 69,
  AvgRowLength = 70,
  Backup = 71,
  Base64 = 72,
  Batch = 73,
  Before = 74,
  Begin = 75,
  Benchmark = 76,
  Between = 77,
  Bfile = 78,
  Big5 = 79,
  Bigint = 80,
  Bin = 81,
  Binary = 82,
  BinaryChecksum = 83,
  BinaryDouble = 84,
  BinaryFloat = 85,
  BinaryInteger = 86,
  Binlog = 87,
  Bit = 88,
  BitAnd = 89,
  BitCount = 90,
  BitLength = 91,
  BitOr = 92,
  BitXor = 93,
  Blackhole = 94,
  Blob = 95,
  Block = 96,
  Body = 97,
  Boolean = 98,
  Both = 99,
  Breadth = 100,
  Break = 101,
  Browse = 102,
  Btree = 103,
  Buffer = 104,
  Bulk = 105,
  BulkLogged = 106,
  By = 107,
  Byte = 108,
  Cache = 109,
  Call = 110,
  Caller = 111,
  Canonical = 112,
  Cascade = 113,
  Cascaded = 114,
  Case = 115,
  Cast = 116,
  Catch = 117,
  Ceil = 118,
  Ceiling = 119,
  Centroid = 120,
  Chain = 121,
  Change = 122,
  Changed = 123,
  Changes = 124,
  Changetable = 125,
  ChangeRetention = 126,
  ChangeTracking = 127,
  Char = 128,
  Character = 129,
  CharacterLength = 130,
  Charset = 131,
  CharCs = 132,
  CharLength = 133,
  Check = 134,
  Checkpoint = 135,
  Checksum = 136,
  ChecksumAgg = 137,
  Chr = 138,
  Cipher = 139,
  Client = 140,
  Clob = 141,
  Close = 142,
  Cluster = 143,
  Clustered = 144,
  Coalesce = 145,
  Code = 146,
  Coercibility = 147,
  Collate = 148,
  Collation = 149,
  Collect = 150,
  Column = 151,
  Columns = 152,
  ColumnFormat = 153,
  Comment = 154,
  Commit = 155,
  Committed = 156,
  Compact = 157,
  Compatibility = 158,
  CompatibilityLevel = 159,
  Compile = 160,
  Completion = 161,
  Compound = 162,
  Compress = 163,
  Compressed = 164,
  Compute = 165,
  Concat = 166,
  ConcatNullYieldsNull = 167,
  ConcatWs = 168,
  Concurrent = 169,
  Condition = 170,
  Connect = 171,
  Connection = 172,
  ConnectionId = 173,
  ConnectByRoot = 174,
  Consistent = 175,
  Constant = 176,
  Constraint = 177,
  Constraints = 178,
  Constructor = 179,
  Containment = 180,
  Contains = 181,
  Containstable = 182,
  Content = 183,
  Context = 184,
  Continue = 185,
  Contributors = 186,
  Control = 187,
  Conv = 188,
  Convert = 189,
  ConvertTz = 190,
  Cookie = 191,
  Copy = 192,
  Corr = 193,
  CorruptXid = 194,
  CorruptXidAll = 195,
  Cos = 196,
  Cost = 197,
  Cot = 198,
  Count = 199,
  CountBig = 200,
  Covar = 201,
  Cp1250 = 202,
  Cp1251 = 203,
  Cp1256 = 204,
  Cp1257 = 205,
  Cp850 = 206,
  Cp852 = 207,
  Cp866 = 208,
  Cp932 = 209,
  Crc32 = 210,
  Create = 211,
  CreateAsymmetricPrivKey = 212,
  CreateAsymmetricPubKey = 213,
  CreateDhParameters = 214,
  CreateDigest = 215,
  Cross = 216,
  Crosses = 217,
  Csv = 218,
  Cube = 219,
  CumeDist = 220,
  Curdate = 221,
  Current = 222,
  CurrentDate = 223,
  CurrentTime = 224,
  CurrentTimestamp = 225,
  CurrentUser = 226,
  Cursor = 227,
  CursorCloseOnCommit = 228,
  CursorDefault = 229,
  Curtime = 230,
  Customdatum = 231,
  Cycle = 232,
  CLetter = 233,
  Data = 234,
  Database = 235,
  Databases = 236,
  Datafile = 237,
  Date = 238,
  Dateadd = 239,
  Datediff = 240,
  Datename = 241,
  Datepart = 242,
  Datetime = 243,
  DateAdd = 244,
  DateCorrelationOptimization = 245,
  DateFormat = 246,
  DateSub = 247,
  Day = 248,
  Dayname = 249,
  Dayofmonth = 250,
  Dayofweek = 251,
  Dayofyear = 252,
  Days = 253,
  DayHour = 254,
  DayMicrosecond = 255,
  DayMinute = 256,
  DaySecond = 257,
  Dbcc = 258,
  Dbtimezone = 259,
  DbChaining = 260,
  DbRoleChange = 261,
  Ddl = 262,
  Deallocate = 263,
  Debug = 264,
  Dec = 265,
  Dec8 = 266,
  Decimal = 267,
  Declare = 268,
  Decode = 269,
  Decompose = 270,
  Decrement = 271,
  Default = 272,
  Defaults = 273,
  DefaultFulltextLanguage = 274,
  DefaultLanguage = 275,
  Deferred = 276,
  Definer = 277,
  Degrees = 278,
  Delay = 279,
  Delayed = 280,
  DelayedDurability = 281,
  DelayKeyWrite = 282,
  Delete = 283,
  Deleted = 284,
  DenseRank = 285,
  Deny = 286,
  Depth = 287,
  Desc = 288,
  Describe = 289,
  DesDecrypt = 290,
  DesEncrypt = 291,
  DesKeyFile = 292,
  Deterministic = 293,
  Dimension = 294,
  Directory = 295,
  DirectoryName = 296,
  Disable = 297,
  Disabled = 298,
  DisableBroker = 299,
  Disassociate = 300,
  Discard = 301,
  Disjoint = 302,
  Disk = 303,
  Distinct = 304,
  Distinctrow = 305,
  Distributed = 306,
  Do = 307,
  Document = 308,
  Double = 309,
  Drop = 310,
  DsintervalUnconstrained = 311,
  Dump = 312,
  Dumpfile = 313,
  Duplicate = 314,
  Dynamic = 315,
  Each = 316,
  Element = 317,
  Else = 318,
  Elseif = 319,
  Elsif = 320,
  Elt = 321,
  Emergency = 322,
  Empty = 323,
  Enable = 324,
  EnableBroker = 325,
  Enclosed = 326,
  Encode = 327,
  Encoding = 328,
  Encrypt = 329,
  Encryption = 330,
  End = 331,
  Endpoint = 332,
  Ends = 333,
  Engine = 334,
  Engines = 335,
  Entityescaping = 336,
  Enum = 337,
  Envelope = 338,
  Equals = 339,
  Err = 340,
  Errlvl = 341,
  Errors = 342,
  ErrorBrokerConversations = 343,
  Escape = 344,
  Escaped = 345,
  Eucjpms = 346,
  Euckr = 347,
  Eur = 348,
  Evalname = 349,
  Even = 350,
  Event = 351,
  Events = 352,
  Every = 353,
  Except = 354,
  Exception = 355,
  Exceptions = 356,
  ExceptionInit = 357,
  Exchange = 358,
  Exclude = 359,
  Exclusive = 360,
  Execute = 361,
  Exists = 362,
  Exit = 363,
  Exp = 364,
  Expand = 365,
  Expire = 366,
  Explain = 367,
  Export = 368,
  ExportSet = 369,
  Extended = 370,
  ExtentSize = 371,
  Exteriorring = 372,
  External = 373,
  Extract = 374,
  Extractvalue = 375,
  Failure = 376,
  False = 377,
  Fast = 378,
  FastForward = 379,
  Federated = 380,
  Fetch = 381,
  Field = 382,
  Fields = 383,
  File = 384,
  Filegroup = 385,
  Filegrowth = 386,
  Filename = 387,
  Filestream = 388,
  Fillfactor = 389,
  Final = 390,
  FindInSet = 391,
  First = 392,
  FirstValue = 393,
  Fixed = 394,
  Float = 395,
  Floor = 396,
  Flush = 397,
  Following = 398,
  Follows = 399,
  For = 400,
  Forall = 401,
  Force = 402,
  Forced = 403,
  Forceseek = 404,
  Foreign = 405,
  Format = 406,
  ForwardOnly = 407,
  Found = 408,
  FoundRows = 409,
  Freetext = 410,
  Freetexttable = 411,
  From = 412,
  FromBase64 = 413,
  FromDays = 414,
  FromUnixtime = 415,
  Full = 416,
  Fullscan = 417,
  Fulltext = 418,
  Function = 419,
  Gb = 420,
  Gb2312 = 421,
  Gbk = 422,
  General = 423,
  Geomcollfromtext = 424,
  Geomcollfromwkb = 425,
  Geometrycollection = 426,
  Geometrycollectionfromtext = 427,
  Geometrycollectionfromwkb = 428,
  Geometryfromtext = 429,
  Geometryfromwkb = 430,
  Geometryn = 431,
  Geometrytype = 432,
  Geomfromtext = 433,
  Geomfromwkb = 434,
  Geostd8 = 435,
  GetFormat = 436,
  GetLock = 437,
  Glength = 438,
  Global = 439,
  Go = 440,
  Goto = 441,
  Grant = 442,
  Grants = 443,
  Greatest = 444,
  Greek = 445,
  Group = 446,
  Grouping = 447,
  GroupingId = 448,
  GroupConcat = 449,
  GtidSubset = 450,
  GtidSubtract = 451,
  Hadr = 452,
  Handler = 453,
  Hash = 454,
  Having = 455,
  Hebrew = 456,
  Help = 457,
  Hex = 458,
  Hide = 459,
  HighPriority = 460,
  HonorBrokerPriority = 461,
  Host = 462,
  Hosts = 463,
  Hour = 464,
  Hours = 465,
  HourMicrosecond = 466,
  HourMinute = 467,
  HourSecond = 468,
  Hp8 = 469,
  Identified = 470,
  Identity = 471,
  Identitycol = 472,
  IdentityInsert = 473,
  If = 474,
  Ifnull = 475,
  Ignore = 476,
  IgnoreNonclusteredColumnstoreIndex = 477,
  Immediate = 478,
  Impersonate = 479,
  Import = 480,
  In = 481,
  Include = 482,
  Including = 483,
  Increment = 484,
  Incremental = 485,
  Indent = 486,
  Index = 487,
  Indexed = 488,
  Indicator = 489,
  Indices = 490,
  Inet6Aton = 491,
  Inet6Ntoa = 492,
  InetAton = 493,
  InetNtoa = 494,
  Infile = 495,
  Infinite = 496,
  InitialSize = 497,
  Inline = 498,
  Inner = 499,
  Innodb = 500,
  Inout = 501,
  Inplace = 502,
  Insensitive = 503,
  Insert = 504,
  Inserted = 505,
  InsertMethod = 506,
  Install = 507,
  Instantiable = 508,
  Instead = 509,
  Instr = 510,
  Int = 511,
  Integer = 512,
  Interiorringn = 513,
  Internal = 514,
  Intersect = 515,
  Intersects = 516,
  Interval = 517,
  Into = 518,
  Invalidate = 519,
  Invoker = 520,
  Is = 521,
  Isclosed = 522,
  Isempty = 523,
  Isnull = 524,
  Iso = 525,
  Isolation = 526,
  Issimple = 527,
  Issuer = 528,
  IsFreeLock = 529,
  IsIpv4 = 530,
  IsIpv4Compat = 531,
  IsIpv4Mapped = 532,
  IsIpv6 = 533,
  IsUsedLock = 534,
  Iterate = 535,
  Java = 536,
  Jis = 537,
  Join = 538,
  Json = 539,
  Kb = 540,
  Keep = 541,
  Keepfixed = 542,
  Key = 543,
  Keybcs2 = 544,
  Keys = 545,
  Keyset = 546,
  KeyBlockSize = 547,
  Kill = 548,
  Koi8R = 549,
  Koi8U = 550,
  Lag = 551,
  Language = 552,
  Last = 553,
  LastInsertId = 554,
  LastValue = 555,
  Latin1 = 556,
  Latin2 = 557,
  Latin5 = 558,
  Latin7 = 559,
  Lcase = 560,
  Lead = 561,
  Leading = 562,
  Least = 563,
  Leave = 564,
  Left = 565,
  Length = 566,
  Less = 567,
  Level = 568,
  Library = 569,
  Like = 570,
  Like2 = 571,
  Like4 = 572,
  Likec = 573,
  Limit = 574,
  Linear = 575,
  Linefromtext = 576,
  Linefromwkb = 577,
  Lineno = 578,
  Lines = 579,
  Linestring = 580,
  Linestringfromtext = 581,
  Linestringfromwkb = 582,
  List = 583,
  Listagg = 584,
  Ln = 585,
  Load = 586,
  LoadFile = 587,
  Local = 588,
  Localtime = 589,
  Localtimestamp = 590,
  Locate = 591,
  Lock = 592,
  Locked = 593,
  LockEscalation = 594,
  Log = 595,
  Log10 = 596,
  Log2 = 597,
  Logfile = 598,
  Login = 599,
  Logoff = 600,
  Logon = 601,
  Logs = 602,
  Long = 603,
  Longblob = 604,
  Longtext = 605,
  Loop = 606,
  Lower = 607,
  LowPriority = 608,
  Lpad = 609,
  Ltrim = 610,
  Macce = 611,
  Macroman = 612,
  Main = 613,
  Makedate = 614,
  Maketime = 615,
  MakeSet = 616,
  Map = 617,
  Mark = 618,
  Master = 619,
  MasterPosWait = 620,
  Match = 621,
  Matched = 622,
  Max = 623,
  Maxdop = 624,
  Maxrecursion = 625,
  Maxsize = 626,
  Maxvalue = 627,
  MaxConnectionsPerHour = 628,
  MaxQueriesPerHour = 629,
  MaxRows = 630,
  MaxSize = 631,
  MaxUpdatesPerHour = 632,
  MaxUserConnections = 633,
  Mb = 634,
  Mbrcontains = 635,
  Mbrdisjoint = 636,
  Mbrequal = 637,
  Mbrintersects = 638,
  Mbroverlaps = 639,
  Mbrtouches = 640,
  Mbrwithin = 641,
  Md5 = 642,
  Measures = 643,
  Median = 644,
  Medium = 645,
  Mediumblob = 646,
  Mediumint = 647,
  Mediumtext = 648,
  Member = 649,
  Memory = 650,
  MemoryOptimizedData = 651,
  Merge = 652,
  Microsecond = 653,
  Mid = 654,
  Min = 655,
  Minus = 656,
  Minute = 657,
  Minutes = 658,
  MinuteMicrosecond = 659,
  MinuteSecond = 660,
  Minvalue = 661,
  MinActiveRowversion = 662,
  MinRows = 663,
  MixedPageAllocation = 664,
  Mlinefromtext = 665,
  Mlinefromwkb = 666,
  Mlslabel = 667,
  Mode = 668,
  Model = 669,
  Modifies = 670,
  Modify = 671,
  Month = 672,
  Monthname = 673,
  Mpointfromtext = 674,
  Mpointfromwkb = 675,
  Mpolyfromtext = 676,
  Mpolyfromwkb = 677,
  MrgMyisam = 678,
  Multilinestring = 679,
  Multilinestringfromtext = 680,
  Multilinestringfromwkb = 681,
  Multipoint = 682,
  Multipointfromtext = 683,
  Multipointfromwkb = 684,
  Multipolygon = 685,
  Multipolygonfromtext = 686,
  Multipolygonfromwkb = 687,
  Multiset = 688,
  MultiUser = 689,
  Mutex = 690,
  Myisam = 691,
  Mysql = 692,
  Name = 693,
  Names = 694,
  NameConst = 695,
  Nan = 696,
  National = 697,
  Natural = 698,
  Naturaln = 699,
  Nav = 700,
  Nchar = 701,
  NcharCs = 702,
  Nclob = 703,
  Ndb = 704,
  Ndbcluster = 705,
  Nested = 706,
  NestedTriggers = 707,
  New = 708,
  NewBroker = 709,
  Next = 710,
  No = 711,
  Noaudit = 712,
  Nocache = 713,
  Nocheck = 714,
  Nocopy = 715,
  Nocount = 716,
  Nocycle = 717,
  Nodegroup = 718,
  Noentityescaping = 719,
  Noexpand = 720,
  Nomaxvalue = 721,
  Nominvalue = 722,
  Nonclustered = 723,
  None = 724,
  NonTransactedAccess = 725,
  Noorder = 726,
  Norecompute = 727,
  Noschemacheck = 728,
  Not = 729,
  Now = 730,
  Nowait = 731,
  NoWait = 732,
  NoWriteToBinlog = 733,
  Ntile = 734,
  Null = 735,
  Nullif = 736,
  Nulls = 737,
  NullLiteral = 738,
  Number = 739,
  Numeric = 740,
  NumericRoundabort = 741,
  Numgeometries = 742,
  Numinteriorrings = 743,
  Numpoints = 744,
  Nvarchar2 = 745,
  Object = 746,
  Oct = 747,
  OctetLength = 748,
  Of = 749,
  Off = 750,
  Offline = 751,
  Offset = 752,
  Offsets = 753,
  Oid = 754,
  Oj = 755,
  Old = 756,
  OldPassword = 757,
  On = 758,
  Online = 759,
  Only = 760,
  Open = 761,
  Opendatasource = 762,
  Openquery = 763,
  Openrowset = 764,
  Openxml = 765,
  Optimistic = 766,
  Optimize = 767,
  Option = 768,
  Optionally = 769,
  Options = 770,
  Or = 771,
  Oradata = 772,
  Ord = 773,
  Order = 774,
  Ordinality = 775,
  Oserror = 776,
  Out = 777,
  Outer = 778,
  Outfile = 779,
  Output = 780,
  Over = 781,
  Overlaps = 782,
  Overriding = 783,
  Owner = 784,
  Package = 785,
  PackKeys = 786,
  PageVerify = 787,
  ParallelEnable = 788,
  Parameterization = 789,
  Parameters = 790,
  Parent = 791,
  Parser = 792,
  Partial = 793,
  Partition = 794,
  Partitioning = 795,
  Partitions = 796,
  Passing = 797,
  Password = 798,
  Path = 799,
  Percent = 800,
  PercentileCont = 801,
  PercentileDisc = 802,
  PercentFound = 803,
  PercentIsopen = 804,
  PercentNotfound = 805,
  PercentRank = 806,
  PercentRowcount = 807,
  PercentRowtype = 808,
  PercentType = 809,
  PerfomanceSchema = 810,
  PeriodAdd = 811,
  PeriodDiff = 812,
  Pi = 813,
  Pipelined = 814,
  Pivot = 815,
  Plan = 816,
  PlsInteger = 817,
  Plugin = 818,
  Plugins = 819,
  Point = 820,
  Pointfromtext = 821,
  Pointfromwkb = 822,
  Pointn = 823,
  Polyfromtext = 824,
  Polyfromwkb = 825,
  Polygon = 826,
  Polygonfromtext = 827,
  Polygonfromwkb = 828,
  Port = 829,
  Position = 830,
  Positive = 831,
  Positiven = 832,
  Pow = 833,
  Power = 834,
  Pragma = 835,
  Preceding = 836,
  Precision = 837,
  Prediction = 838,
  PredictionBounds = 839,
  PredictionCost = 840,
  PredictionDetails = 841,
  PredictionProbability = 842,
  PredictionSet = 843,
  Present = 844,
  Preserve = 845,
  Primary = 846,
  Print = 847,
  Prior = 848,
  Privileges = 849,
  Proc = 850,
  Procedure = 851,
  Process = 852,
  Processlist = 853,
  Profile = 854,
  Profiles = 855,
  Proxy = 856,
  Public = 857,
  Quarter = 858,
  Query = 859,
  Quick = 860,
  Quote = 861,
  QuotedIdentifier = 862,
  Radians = 863,
  Raise = 864,
  Raiserror = 865,
  Rand = 866,
  RandomBytes = 867,
  Range = 868,
  Rank = 869,
  RatioToReport = 870,
  Raw = 871,
  Read = 872,
  Readonly = 873,
  Reads = 874,
  Readtext = 875,
  ReadCommittedSnapshot = 876,
  ReadOnly = 877,
  ReadWrite = 878,
  Real = 879,
  Rebuild = 880,
  Recompile = 881,
  Reconfigure = 882,
  Record = 883,
  Recovery = 884,
  RecursiveTriggers = 885,
  RedoBufferSize = 886,
  Redundant = 887,
  Ref = 888,
  Reference = 889,
  References = 890,
  Referencing = 891,
  Regexp = 892,
  Regr = 893,
  Reject = 894,
  Relative = 895,
  Relay = 896,
  Relaylog = 897,
  Release = 898,
  ReleaseLock = 899,
  ReliesOn = 900,
  Reload = 901,
  Remote = 902,
  Remove = 903,
  Rename = 904,
  Reorganize = 905,
  Repair = 906,
  Repeat = 907,
  Repeatable = 908,
  Replace = 909,
  Replication = 910,
  Require = 911,
  Reset = 912,
  Respect = 913,
  Restore = 914,
  Restrict = 915,
  RestrictedUser = 916,
  RestrictReferences = 917,
  Result = 918,
  ResultCache = 919,
  Return = 920,
  Returning = 921,
  Returns = 922,
  Reuse = 923,
  Reverse = 924,
  Revert = 925,
  Revoke = 926,
  Right = 927,
  Rlike = 928,
  Robust = 929,
  Rollback = 930,
  Rollup = 931,
  Root = 932,
  Round = 933,
  Routine = 934,
  Row = 935,
  Rowcount = 936,
  Rowguid = 937,
  Rowguidcol = 938,
  Rowid = 939,
  Rows = 940,
  RowCount = 941,
  RowFormat = 942,
  RowNumber = 943,
  Rpad = 944,
  Rtrim = 945,
  Rule = 946,
  Rules = 947,
  Sample = 948,
  Save = 949,
  Savepoint = 950,
  Schedule = 951,
  Schema = 952,
  Schemabinding = 953,
  Schemacheck = 954,
  Scn = 955,
  Scroll = 956,
  ScrollLocks = 957,
  Search = 958,
  Second = 959,
  Seconds = 960,
  SecondMicrosecond = 961,
  Security = 962,
  Securityaudit = 963,
  SecToTime = 964,
  Seed = 965,
  Segment = 966,
  Select = 967,
  Self = 968,
  Semantickeyphrasetable = 969,
  Semanticsimilaritydetailstable = 970,
  Semanticsimilaritytable = 971,
  Separator = 972,
  Sequence = 973,
  Sequential = 974,
  Serializable = 975,
  SeriallyReusable = 976,
  Server = 977,
  Servererror = 978,
  Session = 979,
  Sessiontimezone = 980,
  SessionUser = 981,
  Set = 982,
  Sets = 983,
  Settings = 984,
  Setuser = 985,
  Sha = 986,
  Sha1 = 987,
  Sha2 = 988,
  Share = 989,
  Shared = 990,
  Show = 991,
  Showplan = 992,
  Shutdown = 993,
  Siblings = 994,
  Sign = 995,
  Signed = 996,
  Signtype = 997,
  Simple = 998,
  SimpleInteger = 999,
  Sin = 1000,
  Single = 1001,
  SingleUser = 1002,
  Size = 1003,
  Sjis = 1004,
  Skip = 1005,
  Slave = 1006,
  Sleep = 1007,
  Slow = 1008,
  Smallint = 1009,
  Snapshot = 1010,
  Socket = 1011,
  Some = 1012,
  Soname = 1013,
  Soundex = 1014,
  Sounds = 1015,
  Spatial = 1016,
  SpatialWindowMaxCells = 1017,
  Specification = 1018,
  Sql = 1019,
  Sqldata = 1020,
  Sqlerror = 1021,
  Sqlexception = 1022,
  Sqlstate = 1023,
  Sqlwarning = 1024,
  SqlBigResult = 1025,
  SqlBufferResult = 1026,
  SqlCache = 1027,
  SqlCalcFoundRows = 1028,
  SqlNoCache = 1029,
  SqlSmallResult = 1030,
  SqlThreadWaitAfterGtids = 1031,
  Sqrt = 1032,
  Srid = 1033,
  Ssl = 1034,
  Standalone = 1035,
  Start = 1036,
  Starting = 1037,
  Startpoint = 1038,
  Starts = 1039,
  Startup = 1040,
  Statement = 1041,
  StatementId = 1042,
  Static = 1043,
  Statistics = 1044,
  StatsAutoRecalc = 1045,
  StatsPersistent = 1046,
  StatsSamplePages = 1047,
  StatsStream = 1048,
  Status = 1049,
  Std = 1050,
  Stddev = 1051,
  StddevPop = 1052,
  StddevSamp = 1053,
  Stdev = 1054,
  Stdevp = 1055,
  Storage = 1056,
  StraightJoin = 1057,
  Strcmp = 1058,
  String = 1059,
  StrToDate = 1060,
  StArea = 1061,
  StAsbinary = 1062,
  StAstext = 1063,
  StAswkb = 1064,
  StAswkt = 1065,
  StBuffer = 1066,
  StCentroid = 1067,
  StContains = 1068,
  StCrosses = 1069,
  StDifference = 1070,
  StDimension = 1071,
  StDisjoint = 1072,
  StDistance = 1073,
  StEndpoint = 1074,
  StEnvelope = 1075,
  StEquals = 1076,
  StExteriorring = 1077,
  StGeomcollfromtext = 1078,
  StGeomcollfromtxt = 1079,
  StGeomcollfromwkb = 1080,
  StGeometrycollectionfromtext = 1081,
  StGeometrycollectionfromwkb = 1082,
  StGeometryfromtext = 1083,
  StGeometryfromwkb = 1084,
  StGeometryn = 1085,
  StGeometrytype = 1086,
  StGeomfromtext = 1087,
  StGeomfromwkb = 1088,
  StInteriorringn = 1089,
  StIntersection = 1090,
  StIntersects = 1091,
  StIsclosed = 1092,
  StIsempty = 1093,
  StIssimple = 1094,
  StLinefromtext = 1095,
  StLinefromwkb = 1096,
  StLinestringfromtext = 1097,
  StLinestringfromwkb = 1098,
  StNumgeometries = 1099,
  StNuminteriorring = 1100,
  StNuminteriorrings = 1101,
  StNumpoints = 1102,
  StOverlaps = 1103,
  StPointfromtext = 1104,
  StPointfromwkb = 1105,
  StPointn = 1106,
  StPolyfromtext = 1107,
  StPolyfromwkb = 1108,
  StPolygonfromtext = 1109,
  StPolygonfromwkb = 1110,
  StSrid = 1111,
  StStartpoint = 1112,
  StSymdifference = 1113,
  StTouches = 1114,
  StUnion = 1115,
  StWithin = 1116,
  StX = 1117,
  StY = 1118,
  Subdate = 1119,
  Subject = 1120,
  Submultiset = 1121,
  Subpartition = 1122,
  Subpartitions = 1123,
  Substitutable = 1124,
  Substr = 1125,
  Substring = 1126,
  SubstringIndex = 1127,
  Subtime = 1128,
  Subtype = 1129,
  Success = 1130,
  Sum = 1131,
  Super = 1132,
  Suspend = 1133,
  Swe7 = 1134,
  Sysdate = 1135,
  SystemUser = 1136,
  Table = 1137,
  Tables = 1138,
  Tablesample = 1139,
  Tablespace = 1140,
  Take = 1141,
  Tan = 1142,
  TargetRecoveryTime = 1143,
  Tb = 1144,
  Temporary = 1145,
  Temptable = 1146,
  Terminated = 1147,
  Text = 1148,
  TextimageOn = 1149,
  Textsize = 1150,
  Than = 1151,
  The = 1152,
  Then = 1153,
  Throw = 1154,
  Ties = 1155,
  Time = 1156,
  Timediff = 1157,
  Timestamp = 1158,
  Timestampadd = 1159,
  Timestampdiff = 1160,
  TimestampLtzUnconstrained = 1161,
  TimestampTzUnconstrained = 1162,
  TimestampUnconstrained = 1163,
  TimezoneAbbr = 1164,
  TimezoneHour = 1165,
  TimezoneMinute = 1166,
  TimezoneRegion = 1167,
  TimeFormat = 1168,
  TimeToSec = 1169,
  Tinyblob = 1170,
  Tinyint = 1171,
  Tinytext = 1172,
  Tis620 = 1173,
  To = 1174,
  Top = 1175,
  TornPageDetection = 1176,
  Touches = 1177,
  ToBase64 = 1178,
  ToDays = 1179,
  ToSeconds = 1180,
  Traditional = 1181,
  Trailing = 1182,
  Tran = 1183,
  Transaction = 1184,
  TransformNoiseWords = 1185,
  Translate = 1186,
  Treat = 1187,
  Trigger = 1188,
  Triggers = 1189,
  Trim = 1190,
  True = 1191,
  Truncate = 1192,
  Trustworthy = 1193,
  Try = 1194,
  TryConvert = 1195,
  Tsequal = 1196,
  TwoDigitYearCutoff = 1197,
  Type = 1198,
  TypeWarning = 1199,
  Ucase = 1200,
  Ucs2 = 1201,
  Ujis = 1202,
  Unbounded = 1203,
  Uncommitted = 1204,
  Uncompress = 1205,
  UncompressedLength = 1206,
  Undefined = 1207,
  Under = 1208,
  Undo = 1209,
  Undofile = 1210,
  UndoBufferSize = 1211,
  Unhex = 1212,
  Uninstall = 1213,
  Union = 1214,
  Unique = 1215,
  UnixTimestamp = 1216,
  Unknown = 1217,
  Unlimited = 1218,
  Unlock = 1219,
  Unpivot = 1220,
  Unsigned = 1221,
  Until = 1222,
  Update = 1223,
  Updated = 1224,
  Updatetext = 1225,
  Updatexml = 1226,
  Upgrade = 1227,
  Upper = 1228,
  Upsert = 1229,
  Urowid = 1230,
  Usa = 1231,
  Usage = 1232,
  Use = 1233,
  User = 1234,
  UserResources = 1235,
  UseFrm = 1236,
  Using = 1237,
  UtcDate = 1238,
  UtcTime = 1239,
  UtcTimestamp = 1240,
  Utf16 = 1241,
  Utf16Le = 1242,
  Utf32 = 1243,
  Utf8 = 1244,
  Utf8Mb3 = 1245,
  Utf8Mb4 = 1246,
  Uuid = 1247,
  UuidShort = 1248,
  Validate = 1249,
  ValidatePasswordStrength = 1250,
  Value = 1251,
  Values = 1252,
  Var1 = 1253,
  Varbinary = 1254,
  Varchar = 1255,
  Varchar2 = 1256,
  Variable = 1257,
  Variables = 1258,
  Variance = 1259,
  Varp = 1260,
  Varray = 1261,
  Varying = 1262,
  Var2 = 1263,
  VarPop = 1264,
  VarSamp = 1265,
  Version = 1266,
  Versions = 1267,
  View = 1268,
  Views = 1269,
  ViewMetadata = 1270,
  Wait = 1271,
  Waitfor = 1272,
  WaitUntilSqlThreadAfterGtids = 1273,
  Warning = 1274,
  Warnings = 1275,
  Week = 1276,
  Weekday = 1277,
  Weekofyear = 1278,
  WeightString = 1279,
  Wellformed = 1280,
  When = 1281,
  Whenever = 1282,
  Where = 1283,
  While = 1284,
  With = 1285,
  Within = 1286,
  Work = 1287,
  Wrapper = 1288,
  Write = 1289,
  Writetext = 1290,
  X509 = 1291,
  Xml = 1292,
  Xmlagg = 1293,
  Xmlattributes = 1294,
  Xmlcast = 1295,
  Xmlcolattval = 1296,
  Xmlelement = 1297,
  Xmlexists = 1298,
  Xmlforest = 1299,
  Xmlnamespaces = 1300,
  Xmlparse = 1301,
  Xmlpi = 1302,
  Xmlquery = 1303,
  Xmlroot = 1304,
  Xmlserialize = 1305,
  Xmltable = 1306,
  Xor = 1307,
  Year = 1308,
  Yearweek = 1309,
  YearMonth = 1310,
  Yes = 1311,
  YmintervalUnconstrained = 1312,
  Zerofill = 1313,
  Zone = 1314,
  DollarAction = 1315,
  YFunction = 1316,
  XFunction = 1317,
  AKeyword = 1318,
  StarSymbol = 1319,
  DivideSymbol = 1320,
  ModuleSymbol = 1321,
  PlusSymbol = 1322,
  MinusSymbol = 1323,
  NullsaveNotEqualSymbol = 1324,
  GreaterEqualSymbol = 1325,
  LessEqualSymbol = 1326,
  NotEqualSymbol1 = 1327,
  NotEqualSymbol2 = 1328,
  EqualSymbol = 1329,
  GreaterSymbol = 1330,
  LessSymbol = 1331,
  ExclamationSymbol = 1332,
  BitNotOpSymbol = 1333,
  BitOrOpSymbol = 1334,
  BitAndOpSymbol = 1335,
  BitXorOpSymbol = 1336,
  RshiftOpSymbol = 1337,
  LshiftOpSymbol = 1338,
  LogicalOrOpSymbol = 1339,
  LocicalAndOpSymbol = 1340,
  DotSymbol = 1341,
  LrBracketSymbol = 1342,
  RrBracketSymbol = 1343,
  CommaSymbol = 1344,
  SemiSymbol = 1345,
  AtSignSymbol = 1346,
  SingleQuoteSymbol = 1347,
  DoubleQuoteSymbol = 1348,
  ReverseQuoteSymbol = 1349,
  ColonSymbol = 1350,
  SharpSymbol = 1351,
  DollarSymbol = 1352,
  UnderlineSymbol = 1353,
  DoublePeriodSymbol = 1354,
  DoubleAsteriskSymbol = 1355,
  Div = 1356,
  Mod = 1357,
  VarAssignSymbol = 1358,
  PlusAssignSymbol = 1359,
  MinusAssignSymbol = 1360,
  MultAssignSymbol = 1361,
  DivAssignSymbol = 1362,
  ModAssignSymbol = 1363,
  AndAssignSymbol = 1364,
  XorAssignSymbol = 1365,
  OrAssignSymbol = 1366,
  CharsetReverseQouteString = 1367,
  FilesizeLiteral = 1368,
  StartNationalStringLiteral = 1369,
  StringLiteral = 1370,
  DecimalLiteral = 1371,
  HexadecimalLiteral = 1372,
  RealLiteral = 1373,
  NullSpecLiteral = 1374,
  BitString = 1375,
  StringCharsetName = 1376,
  DotId = 1377,
  LabelMark = 1378,
  Id = 1379,
  DoubleQuoteId = 1380,
  SquareBracketId = 1381,
  ReverseQuoteId = 1382,
  StringUserName = 1383,
  LocalId = 1384,
  GlobalId = 1385,
  Error = 1386
};

class Sql final : public AntlrLanguageProvider {
public:
  Sql() = default;

  std::pair<std::string, bool> trySanitize(const std::string &text, const Token &context) const override;

  std::string tokenTypeToString(TokenType type) const override;

private:
  std::pair<std::string, bool> trySqlEncode(const std::string &text, SqlTokenType tokenType) const;

  Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const override;

  bool isTrivial(TokenType type, const std::string &text) const override;

  TokenType convertAntlrTokenType(size_t antlrTokenType) const override;

  std::unique_ptr<antlr4::Lexer> createLexer(const std::string &text, antlr4::CharStream *charStream) const override;
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_SQL_H
