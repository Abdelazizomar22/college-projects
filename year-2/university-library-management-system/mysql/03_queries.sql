-- MySQL: useful queries

-- Available books
SELECT book_id, isbn, title, author, copies_available
FROM books
WHERE copies_available > 0
ORDER BY title;

-- Overdue loans (not returned)
SELECT l.loan_id, m.full_name, b.title, l.borrow_date, l.due_date
FROM loans l
JOIN members m ON m.member_id = l.member_id
JOIN books b ON b.book_id = l.book_id
WHERE l.return_date IS NULL AND l.due_date < CURDATE()
ORDER BY l.due_date;

-- Members with unpaid fines
SELECT m.member_id, m.full_name, f.amount
FROM fines f
JOIN loans l ON l.loan_id = f.loan_id
JOIN members m ON m.member_id = l.member_id
WHERE f.paid = FALSE
ORDER BY f.amount DESC;

