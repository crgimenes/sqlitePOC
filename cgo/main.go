package main

import (
	"crypto/rand"
	"fmt"
	"time"

	"github.com/jmoiron/sqlx"
	_ "github.com/mattn/go-sqlite3"
)

// var connectionString = `file:database.db?cache=shared&mode=rwc&_journal_mode=WAL&_busy_timeout=1000000`
var connectionString = "database.db"

// var connectionString = ":memory:"

type Database struct {
	db *sqlx.DB
}

func New() (*Database, error) {
	db, err := sqlx.Open("sqlite3", connectionString)
	if err != nil {
		return nil, err
	}

	tableExists, err := tableExists(db, "logs")
	if err != nil {
		return nil, err
	}

	if !tableExists {

		sql := `CREATE TABLE logs (
			sender TEXT,
			body TEXT
			);`
		_, err = db.Exec(sql)
		if err != nil {
			return nil, err
		}

	}
	return &Database{
		db: db,
	}, nil
}

func tableExists(db *sqlx.DB, tableName string) (bool, error) {
	var count int
	err := db.Get(&count, "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name=$1", tableName)
	if err != nil {
		return false, err
	}
	return count == 1, nil
}

func (d *Database) Save(sender, body string) error {
	tx, err := d.db.Begin()
	if err != nil {
		return err
	}

	// INSERT INTO logs (sender,body) VALUES ('cesar gimenes', 'qualquer coisa la no meio');

	sql := `INSERT INTO logs (
        sender, 	-- 1
        body 		-- 2
        ) VALUES ($1, $2);`
	_, err = tx.Exec(sql,
		sender, // 1
		body,   // 2
	)
	if err != nil {
		tx.Rollback()
		return err
	}
	return tx.Commit()
}

func randomString() string {
	b := make([]byte, 10)
	rand.Read(b)
	return fmt.Sprintf("%x", b)
}

func main() {

	t := time.Now()

	db, err := New()
	if err != nil {
		panic(err)
	}

	for i := 0; i < 1000; i++ {
		err = db.Save(
			fmt.Sprintf("sender %d %v", i, randomString()),
			fmt.Sprintf("body %d %v", i, randomString()),
		)
		if err != nil {
			panic(err)
		}

	}

	//db.db.Close()

	var name string
	err = db.db.Get(&name, "SELECT name FROM sqlite_master  WHERE type = 'table' AND name = 'sqlite_stat1';")
	if err != nil {
		panic(err)
	}
	fmt.Println(name)

	tFinal := time.Since(t)

	fmt.Printf("Tempo por registro: %v\n", tFinal/1000)

	//db, err := New()
	// print count
	var count int
	err = db.db.Get(&count, "SELECT COUNT(*) FROM logs;")
	if err != nil {
		panic(err)
	}

	db.db.Close()

	fmt.Println(count)

}
