-- SQL Server: schema

IF OBJECT_ID('dbo.Fines', 'U') IS NOT NULL DROP TABLE dbo.Fines;
IF OBJECT_ID('dbo.Loans', 'U') IS NOT NULL DROP TABLE dbo.Loans;
IF OBJECT_ID('dbo.Books', 'U') IS NOT NULL DROP TABLE dbo.Books;
IF OBJECT_ID('dbo.Members', 'U') IS NOT NULL DROP TABLE dbo.Members;
GO

CREATE TABLE dbo.Members (
  member_id INT IDENTITY(1,1) PRIMARY KEY,
  full_name NVARCHAR(200) NOT NULL,
  email NVARCHAR(200) NOT NULL UNIQUE,
  phone NVARCHAR(40) NULL,
  created_at DATETIME2 NOT NULL DEFAULT SYSUTCDATETIME()
);
GO

CREATE TABLE dbo.Books (
  book_id INT IDENTITY(1,1) PRIMARY KEY,
  isbn NVARCHAR(40) NOT NULL UNIQUE,
  title NVARCHAR(300) NOT NULL,
  author NVARCHAR(200) NOT NULL,
  copies_total INT NOT NULL CHECK (copies_total >= 0),
  copies_available INT NOT NULL CHECK (copies_available >= 0),
  created_at DATETIME2 NOT NULL DEFAULT SYSUTCDATETIME()
);
GO

CREATE TABLE dbo.Loans (
  loan_id INT IDENTITY(1,1) PRIMARY KEY,
  member_id INT NOT NULL,
  book_id INT NOT NULL,
  borrow_date DATE NOT NULL,
  due_date DATE NOT NULL,
  return_date DATE NULL,
  created_at DATETIME2 NOT NULL DEFAULT SYSUTCDATETIME(),
  CONSTRAINT FK_Loans_Members FOREIGN KEY (member_id) REFERENCES dbo.Members(member_id),
  CONSTRAINT FK_Loans_Books FOREIGN KEY (book_id) REFERENCES dbo.Books(book_id)
);
GO

CREATE TABLE dbo.Fines (
  fine_id INT IDENTITY(1,1) PRIMARY KEY,
  loan_id INT NOT NULL UNIQUE,
  amount DECIMAL(10,2) NOT NULL CHECK (amount >= 0),
  paid BIT NOT NULL DEFAULT 0,
  created_at DATETIME2 NOT NULL DEFAULT SYSUTCDATETIME(),
  CONSTRAINT FK_Fines_Loans FOREIGN KEY (loan_id) REFERENCES dbo.Loans(loan_id)
);
GO

