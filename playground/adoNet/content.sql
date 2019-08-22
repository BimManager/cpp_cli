USE DCV_DB
GO
CREATE TABLE [dbo].[Content]
(
 [StoryID] [int] IDENTITY(1,1) NOT NULL,
 [AuthorID] [int] NOT NULL,
 [Headline] [varchar](80) NOT NULL,
 [Story] [text] NOT NULL,
 CONSTRAINT [PK_Content] PRIMARY KEY CLUSTERED
 (
   [StoryID] ASC
 )
 WITH
 (
  PAD_INDEX = OFF,
  STATISTICS_NORECOMPUTE = OFF,
  IGNORE_DUP_KEY = OFF,
  ALLOW_ROW_LOCKS = ON,
  ALLOW_PAGE_LOCKS = ON
 ) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
