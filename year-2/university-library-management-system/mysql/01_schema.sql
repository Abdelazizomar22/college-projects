-- MySQL: schema

DROP TABLE IF EXISTS fines;
DROP TABLE IF EXISTS loans;
DROP TABLE IF EXISTS books;
DROP TABLE IF EXISTS members;

CREATE TABLE members (
  member_id INT AUTO_INCREMENT PRIMARY KEY,
  full_name VARCHAR(200) NOT NULL,
  email VARCHAR(200) NOT NULL UNIQUE,
  phone VARCHAR(40) NULL,
  created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE books (
  book_id INT AUTO_INCREMENT PRIMARY KEY,
  isbn VARCHAR(40) NOT NULL UNIQUE,
  title VARCHAR(300) NOT NULL,
  author VARCHAR(200) NOT NULL,
  copies_total INT NOT NULL CHECK (copies_total >= 0),
  copies_available INT NOT NULL CHECK (copies_available >= 0),
  created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE loans (
  loan_id INT AUTO_INCREMENT PRIMARY KEY,
  member_id INT NOT NULL,
  book_id INT NOT NULL,
  borrow_date DATE NOT NULL,
  due_date DATE NOT NULL,
  return_date DATE NULL,
  created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT fk_loans_members FOREIGN KEY (member_id) REFERENCES members(member_id),
  CONSTRAINT fk_loans_books FOREIGN KEY (book_id) REFERENCES books(book_id)
);

CREATE TABLE fines (
  fine_id INT AUTO_INCREMENT PRIMARY KEY,
  loan_id INT NOT NULL UNIQUE,
  amount DECIMAL(10,2) NOT NULL CHECK (amount >= 0),
  paid BOOLEAN NOT NULL DEFAULT FALSE,
  created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT fk_fines_loans FOREIGN KEY (loan_id) REFERENCES loans(loan_id)
);

