-- SQL Server: useful queries

-- Available books
SELECT book_id, isbn, title, author, copies_available
FROM dbo.Books
WHERE copies_available > 0
ORDER BY title;

-- Overdue loans (not returned)
SELECT l.loan_id, m.full_name, b.title, l.borrow_date, l.due_date
FROM dbo.Loans l
JOIN dbo.Members m ON m.member_id = l.member_id
JOIN dbo.Books b ON b.book_id = l.book_id
WHERE l.return_date IS NULL AND l.due_date < CAST(GETDATE() AS DATE)
ORDER BY l.due_date;

-- Members with unpaid fines
SELECT m.member_id, m.full_name, f.amount
FROM dbo.Fines f
JOIN dbo.Loans l ON l.loan_id = f.loan_id
JOIN dbo.Members m ON m.member_id = l.member_id
WHERE f.paid = 0
ORDER BY f.amount DESC;

