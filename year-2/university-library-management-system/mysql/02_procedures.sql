-- MySQL: stored procedures

DROP PROCEDURE IF EXISTS sp_borrow_book;
DROP PROCEDURE IF EXISTS sp_return_book;

DELIMITER //

CREATE PROCEDURE sp_borrow_book(
  IN p_member_id INT,
  IN p_book_id INT,
  IN p_borrow_date DATE,
  IN p_loan_days INT
)
BEGIN
  DECLARE v_available INT;
  IF p_borrow_date IS NULL THEN
    SET p_borrow_date = CURDATE();
  END IF;
  IF p_loan_days IS NULL THEN
    SET p_loan_days = 14;
  END IF;

  SELECT copies_available INTO v_available FROM books WHERE book_id = p_book_id FOR UPDATE;
  IF v_available IS NULL THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Book not found';
  END IF;
  IF v_available <= 0 THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'No copies available';
  END IF;

  UPDATE books SET copies_available = copies_available - 1 WHERE book_id = p_book_id;

  INSERT INTO loans(member_id, book_id, borrow_date, due_date)
  VALUES(p_member_id, p_book_id, p_borrow_date, DATE_ADD(p_borrow_date, INTERVAL p_loan_days DAY));

  SELECT LAST_INSERT_ID() AS loan_id;
END//

CREATE PROCEDURE sp_return_book(
  IN p_loan_id INT,
  IN p_return_date DATE,
  IN p_fine_per_day DECIMAL(10,2)
)
BEGIN
  DECLARE v_book_id INT;
  DECLARE v_due_date DATE;
  DECLARE v_old_return DATE;
  DECLARE v_days_overdue INT;

  IF p_return_date IS NULL THEN
    SET p_return_date = CURDATE();
  END IF;
  IF p_fine_per_day IS NULL THEN
    SET p_fine_per_day = 2.00;
  END IF;

  SELECT book_id, due_date, return_date INTO v_book_id, v_due_date, v_old_return
  FROM loans WHERE loan_id = p_loan_id FOR UPDATE;

  IF v_book_id IS NULL THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Loan not found';
  END IF;
  IF v_old_return IS NOT NULL THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Loan already returned';
  END IF;

  UPDATE loans SET return_date = p_return_date WHERE loan_id = p_loan_id;
  UPDATE books SET copies_available = copies_available + 1 WHERE book_id = v_book_id;

  SET v_days_overdue = DATEDIFF(p_return_date, v_due_date);
  IF v_days_overdue > 0 THEN
    INSERT INTO fines(loan_id, amount, paid)
    VALUES(p_loan_id, v_days_overdue * p_fine_per_day, FALSE)
    ON DUPLICATE KEY UPDATE amount = v_days_overdue * p_fine_per_day;
  END IF;
END//

DELIMITER ;

