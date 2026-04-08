-- SQL Server: stored procedures

IF OBJECT_ID('dbo.sp_BorrowBook', 'P') IS NOT NULL DROP PROCEDURE dbo.sp_BorrowBook;
IF OBJECT_ID('dbo.sp_ReturnBook', 'P') IS NOT NULL DROP PROCEDURE dbo.sp_ReturnBook;
GO

CREATE PROCEDURE dbo.sp_BorrowBook
  @MemberId INT,
  @BookId INT,
  @BorrowDate DATE = NULL,
  @LoanDays INT = 14
AS
BEGIN
  SET NOCOUNT ON;
  IF @BorrowDate IS NULL SET @BorrowDate = CAST(GETDATE() AS DATE);

  DECLARE @available INT;
  SELECT @available = copies_available FROM dbo.Books WITH (UPDLOCK, ROWLOCK) WHERE book_id = @BookId;
  IF @available IS NULL
    THROW 50000, 'Book not found', 1;
  IF @available <= 0
    THROW 50001, 'No copies available', 1;

  UPDATE dbo.Books SET copies_available = copies_available - 1 WHERE book_id = @BookId;

  INSERT INTO dbo.Loans (member_id, book_id, borrow_date, due_date)
  VALUES (@MemberId, @BookId, @BorrowDate, DATEADD(DAY, @LoanDays, @BorrowDate));

  SELECT SCOPE_IDENTITY() AS loan_id;
END
GO

CREATE PROCEDURE dbo.sp_ReturnBook
  @LoanId INT,
  @ReturnDate DATE = NULL,
  @FinePerDay DECIMAL(10,2) = 2.00
AS
BEGIN
  SET NOCOUNT ON;
  IF @ReturnDate IS NULL SET @ReturnDate = CAST(GETDATE() AS DATE);

  DECLARE @BookId INT, @DueDate DATE, @OldReturn DATE;
  SELECT @BookId = book_id, @DueDate = due_date, @OldReturn = return_date
  FROM dbo.Loans WITH (UPDLOCK, ROWLOCK)
  WHERE loan_id = @LoanId;

  IF @BookId IS NULL
    THROW 50002, 'Loan not found', 1;

  IF @OldReturn IS NOT NULL
    THROW 50003, 'Loan already returned', 1;

  UPDATE dbo.Loans SET return_date = @ReturnDate WHERE loan_id = @LoanId;
  UPDATE dbo.Books SET copies_available = copies_available + 1 WHERE book_id = @BookId;

  DECLARE @days_overdue INT = DATEDIFF(DAY, @DueDate, @ReturnDate);
  IF @days_overdue > 0
  BEGIN
    MERGE dbo.Fines AS target
    USING (SELECT @LoanId AS loan_id) AS src
    ON (target.loan_id = src.loan_id)
    WHEN MATCHED THEN
      UPDATE SET amount = CAST(@days_overdue AS DECIMAL(10,2)) * @FinePerDay
    WHEN NOT MATCHED THEN
      INSERT (loan_id, amount, paid) VALUES (@LoanId, CAST(@days_overdue AS DECIMAL(10,2)) * @FinePerDay, 0);
  END
END
GO

