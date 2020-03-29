package main

import (
  "log"
  "net/http"
  "os"
)

func main() {
  http.Handle("/", http.FileServer(http.Dir("./static")))
  http.Handle("/downloads/",
    http.StripPrefix("/downloads", http.FileServer(http.Dir(os.Getenv("HOME") + "/Downloads"))),
  )

  log.Println("Listening on :5000...")
  err := http.ListenAndServe(":5000", nil)
  if err != nil {
    log.Fatal(err)
  }
}
