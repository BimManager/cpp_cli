-- Populate Authors table
SET IDENTITY_INSERT [dbo].[Authors] OFF
GO
INSERT INTO [dbo].[Authors] ([LastName], [FirstName])
VALUES ("Doors", "Bill")
INSERT INTO [dbo].[Authors] ([LastName], [FirstName])
VALUES ("Ellidaughter", "Larry")
INSERT INTO [dbo].[Authors] ([LastName], [FirstName])
VALUES ("Fraser", "Stephen")
GO
