CREATE TABLE [dbo].[Authors]
(
 [AuthorID] [int] IDENTITY(1,1) NOT NULL,
 [LastName] [varchar](50) NOT NULL,
 [FirstName] [varchar](50) NOT NULL,
 CONSTRAINT [PK_Authors] PRIMARY KEY CLUSTERED
 (
  [AuthorID] ASC
 )
 WITH
 (
  PAD_INDEX = OFF,
  STATISTICS_NORECOMPUTE = OFF,
  IGNORE_DUP_KEY = OFF,
  ALLOW_ROW_LOCKS = ON,
  ALLOW_PAGE_LOCKS = ON
 ) ON [PRIMARY]
) ON [PRIMARY]