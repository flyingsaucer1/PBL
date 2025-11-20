const express = require("express");
const { spawn } = require("child_process");
const path = require("path");          // 🔥 ADD THIS
const app = express();
const PORT = 3000;

app.use(express.json());

// CORS for all routes
app.use((req, res, next) => {
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader("Access-Control-Allow-Methods", "GET,POST,DELETE,OPTIONS"); // 🔥 ADD DELETE
  res.setHeader("Access-Control-Allow-Headers", "Content-Type");
  if (req.method === "OPTIONS") return res.sendStatus(200);
  next();
});


/* ===============================
   AUTH API
   =============================== */

// POST /register
app.post("/register", (req, res) => {
  const { username, email, password, role } = req.body;

  if (!username || !email || !password || !role) {
    return res.status(400).json({ success: false, message: "Missing required fields." });
  }

  const child = spawn(".\\auth_api.exe", ["register"]);

  const line = `${username}|${password}|${email}|${role}\n`;
  child.stdin.write(line);
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (register) error:", errorOutput);
      return res
        .status(500)
        .json({ success: false, message: "C backend error", detail: errorOutput });
    }

    try {
      const result = JSON.parse(output);
      return res.json(result);
    } catch (e) {
      console.error("JSON parse error (register):", e, output);
      return res
        .status(500)
        .json({ success: false, message: "Invalid JSON from auth backend" });
    }
  });
});

// POST /login
app.post("/login", (req, res) => {
  const { username, password } = req.body;

  if (!username || !password) {
    return res
      .status(400)
      .json({ success: false, message: "Missing username or password." });
  }

  const child = spawn(".\\auth_api.exe", ["login"]);

  const line = `${username}|${password}\n`;
  child.stdin.write(line);
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (login) error:", errorOutput);
      return res
        .status(500)
        .json({ success: false, message: "C backend error", detail: errorOutput });
    }

    try {
      const result = JSON.parse(output);
      if (!result.success) {
        return res.json({ success: false, message: result.message });
      }

      // Simple token/userId generation on Node side
      const token = "token-" + Date.now();
      const userId = username;

      return res.json({
        success: true,
        message: result.message,
        token,
        role: result.role || "student",
        userId,
      });
    } catch (e) {
      console.error("JSON parse error (login):", e, output);
      return res
        .status(500)
        .json({ success: false, message: "Invalid JSON from auth backend" });
    }
  });
});

/* ===============================
   NOTES API
   =============================== */

app.post("/api/notes", (req, res) => {
  const { title, subject, content, faculty } = req.body;

  if (!title || !subject || !content || !faculty) {
    return res.status(400).json({ error: "Missing required fields" });
  }

  const child = spawn(".\\notes_api.exe", ["add"]);
  const line = `${title}|${subject}|${content}|${faculty}\n`;
  child.stdin.write(line);
  child.stdin.end();

  let errorOutput = "";
  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (notes add) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }
    return res.json({ status: "ok" });
  });
});

app.get("/api/notes", (req, res) => {
  const child = spawn(".\\notes_api.exe", ["list"]);

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (notes list) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }

    try {
      const json = JSON.parse(output);
      return res.json(json);
    } catch (e) {
      console.error("JSON parse error (notes list):", e, output);
      return res
        .status(500)
        .json({ error: "Invalid JSON from C backend (notes)" });
    }
  });
});

/* ===============================
   COMMUNITY API
   =============================== */

// GET /api/posts -> list all posts
app.get("/api/posts", (req, res) => {
  const child = spawn(".\\community_api.exe", ["list"]);

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (community list) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }

    try {
      const json = JSON.parse(output);
      return res.json(json);
    } catch (e) {
      console.error("JSON parse error (community list):", e, output);
      return res
        .status(500)
        .json({ error: "Invalid JSON from C backend (community list)" });
    }
  });
});

// POST /api/posts -> create new post
app.post("/api/posts", (req, res) => {
  const { title, author, content } = req.body;

  if (!title || !author || !content) {
    return res.status(400).json({ error: "Missing required fields" });
  }

  const child = spawn(".\\community_api.exe", ["addPost"]);
  const line = `${title}|${author}|${content}\n`;
  child.stdin.write(line);
  child.stdin.end();

  let errorOutput = "";
  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (addPost) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }
    return res.json({ status: "ok" });
  });
});

// GET /api/posts/:title -> single post + comments
app.get("/api/posts/:title", (req, res) => {
  const title = req.params.title;

  const child = spawn(".\\community_api.exe", ["getPost"]);
  child.stdin.write(title + "\n");
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (getPost) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }

    try {
      const obj = JSON.parse(output);
      if (obj === null) {
        return res.status(404).json({ error: "Post not found" });
      }
      return res.json(obj);
    } catch (e) {
      console.error("JSON parse error (getPost):", e, output);
      return res
        .status(500)
        .json({ error: "Invalid JSON from C backend (getPost)" });
    }
  });
});

// POST /api/posts/:title/comments -> add comment
app.post("/api/posts/:title/comments", (req, res) => {
  const postTitle = req.params.title;
  const { author, text } = req.body;

  if (!author || !text) {
    return res.status(400).json({ error: "Missing required fields" });
  }

  const child = spawn(".\\community_api.exe", ["addComment"]);
  const line = `${postTitle}|${author}|${text}\n`;
  child.stdin.write(line);
  child.stdin.end();

  let errorOutput = "";
  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (addComment) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }
    return res.json({ status: "ok" });
  });
});
/* ===============================
   DOUBT API
   =============================== */

// POST /api/doubts -> add a new doubt (student)
app.post("/api/doubts", (req, res) => {
  const { studentName, subject, doubtText } = req.body;

  if (!studentName || !subject || !doubtText) {
    return res.status(400).json({ error: "Missing required fields" });
  }

  const child = spawn(".\\doubt_api.exe", ["add"]);
  const line = `${studentName}|${subject}|${doubtText}\n`;
  child.stdin.write(line);
  child.stdin.end();

  let errorOutput = "";
  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (doubt add) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }
    return res.json({ status: "ok" });
  });
});

// GET /api/doubts
//  - /api/doubts?status=pending   -> pending doubts
//  - /api/doubts?status=solved    -> solved doubts
//  - /api/doubts?student=Name     -> doubts for that student
//  - /api/doubts                  -> all doubts
app.get("/api/doubts", (req, res) => {
  const { status, student } = req.query;

  let args;
  let needsStdin = false;
  let stdinLine = "";

  if (student) {
    args = ["listMy"];
    needsStdin = true;
    stdinLine = student + "\n";
  } else if (status === "pending") {
    args = ["listPending"];
  } else if (status === "solved") {
    args = ["listSolved"];
  } else {
    args = ["list"];
  }

  const child = spawn(".\\doubt_api.exe", args);

  if (needsStdin) {
    child.stdin.write(stdinLine);
  }
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (doubt list) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }

    try {
      const json = JSON.parse(output);
      return res.json(json);
    } catch (e) {
      console.error("JSON parse error (doubt list):", e, output);
      return res
        .status(500)
        .json({ error: "Invalid JSON from C backend (doubts)" });
    }
  });
});

// POST /api/doubts/solve -> faculty solves a doubt
app.post("/api/doubts/solve", (req, res) => {
  const { studentName, facultyName, solution } = req.body;

  if (!studentName || !facultyName || !solution) {
    return res.status(400).json({ error: "Missing required fields" });
  }

  const child = spawn(".\\doubt_api.exe", ["solve"]);
  const line = `${studentName}|${facultyName}|${solution}\n`;
  child.stdin.write(line);
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (doubt solve) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }

    try {
      const json = JSON.parse(output);
      return res.json(json);
    } catch (e) {
      console.error("JSON parse error (doubt solve):", e, output);
      return res
        .status(500)
        .json({ error: "Invalid JSON from C backend (doubt solve)" });
    }
  });
});
/* ===============================
   QUIZ API
   =============================== */

// GET /api/quizzes -> list all quizzes
app.get("/api/quizzes", (req, res) => {
  const child = spawn(".\\quiz_api.exe", ["list"]);

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (quiz list) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }

    try {
      const json = JSON.parse(output || "[]");
      return res.json(json);
    } catch (e) {
      console.error("JSON parse error (quiz list):", e, output);
      return res
        .status(500)
        .json({ error: "Invalid JSON from C backend (quizzes)" });
    }
  });
});

// GET /api/quizzes/:title -> one quiz with all questions
app.get("/api/quizzes/:title", (req, res) => {
  const title = req.params.title;

  const child = spawn(".\\quiz_api.exe", ["get"]);
  child.stdin.write(title + "\n");
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (quiz get) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }

    try {
      const trimmed = output.trim();
      if (trimmed === "null" || trimmed === "") {
        return res.status(404).json({ error: "Quiz not found" });
      }
      const json = JSON.parse(trimmed);
      return res.json(json);
    } catch (e) {
      console.error("JSON parse error (quiz get):", e, output);
      return res
        .status(500)
        .json({ error: "Invalid JSON from C backend (quiz get)" });
    }
  });
});

/* ===============================
   FACULTY API
   =============================== */

// GET /api/faculty -> list all faculty as JSON
app.get("/api/faculty", (req, res) => {
  const child = spawn(".\\faculty_api.exe", ["list"]);

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (faculty list) error:", errorOutput);
      return res
        .status(500)
        .json({ error: "C backend error", detail: errorOutput });
    }

    try {
      const json = JSON.parse(output);
      return res.json(json);
    } catch (e) {
      console.error("JSON parse error (faculty list):", e, output);
      return res
        .status(500)
        .json({ error: "Invalid JSON from C backend (faculty)" });
    }
  });
});
/* ===============================
   SYLLABUS API
   =============================== */

// GET all syllabus
app.get("/api/syllabus", (req, res) => {
  const child = spawn(".\\syllabus_api.exe", ["list"]);
  let output = "";

  child.stdout.on("data", d => output += d.toString());

  child.on("close", () => {
    try {
      res.json(JSON.parse(output));
    } catch {
      res.status(500).json({ error: "Invalid JSON from syllabus backend" });
    }
  });
});

// POST new syllabus
app.post("/api/syllabus", (req, res) => {
  const { subject, units } = req.body;

  const child = spawn(".\\syllabus_api.exe", ["add"]);
  child.stdin.write(`${subject}|${units}\n`);
  child.stdin.end();

  let output = "";

  child.stdout.on("data", d => output += d.toString());

  child.on("close", () => res.json(JSON.parse(output)));
});

// DELETE a syllabus
app.delete("/api/syllabus/:subject", (req, res) => {
  const child = spawn(".\\syllabus_api.exe", ["remove"]);
  child.stdin.write(req.params.subject + "\n");
  child.stdin.end();

  let output = "";
  child.stdout.on("data", d => output += d.toString());
  child.on("close", () => res.json(JSON.parse(output)));
});

// ADD A BOOK
app.post("/api/syllabus/:subject/books", (req, res) => {
  const { title, author } = req.body;

  const child = spawn(".\\syllabus_api.exe", ["addBook"]);
  child.stdin.write(`${req.params.subject}|${title}|${author}\n`);
  child.stdin.end();

  let output = "";
  child.stdout.on("data", d => output += d.toString());
  child.on("close", () => res.json(JSON.parse(output)));
});


// POST /api/faculty -> add new faculty
app.post("/api/faculty", (req, res) => {
  const { name, facultyId, department, subject, email } = req.body;

  if (!name || !facultyId || !department || !subject || !email) {
    return res.status(400).json({ success: false, message: "Missing required fields." });
  }

  const child = spawn(".\\faculty_api.exe", ["add"]);
  const line = `${name}|${facultyId}|${department}|${subject}|${email}\n`;
  child.stdin.write(line);
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", (data) => {
    output += data.toString();
  });

  child.stderr.on("data", (data) => {
    errorOutput += data.toString();
  });

  child.on("close", (code) => {
    if (code !== 0) {
      console.error("C backend (faculty add) error:", errorOutput);
      return res
        .status(500)
        .json({ success: false, message: "C backend error", detail: errorOutput });
    }

    try {
      const result = JSON.parse(output);
      return res.json(result);
    } catch (e) {
      console.error("JSON parse error (faculty add):", e, output);
      return res
        .status(500)
        .json({ success: false, message: "Invalid JSON from faculty backend" });
    }
  });
});

// DELETE /api/faculty/:id -> remove by facultyId
app.delete('/api/faculty/:facultyId', (req, res) => {
    const facultyId = req.params.facultyId;

    const exePath = path.join(__dirname, 'faculty_api.exe');
    const child = spawn(exePath, ['remove', facultyId]);

    let output = '';
    let errorOutput = '';

    child.stdout.on('data', chunk => {
        output += chunk.toString();
    });

    child.stderr.on('data', chunk => {
        errorOutput += chunk.toString();
        console.error('[faculty_api remove stderr]', chunk.toString());
    });

    child.on('close', (code) => {
        if (errorOutput && !output) {
            return res.status(500).json({
                success: false,
                message: 'Error from C backend (faculty remove).',
                details: errorOutput
            });
        }
        try {
            const json = JSON.parse(output || '{}');
            res.json(json);
        } catch {
            res.status(500).json({
                success: false,
                message: 'Invalid JSON from C backend (faculty remove).',
                raw: output
            });
        }
    });
});
/* ===============================
   SYLLABUS API
   =============================== */

/* 
---------------------------------
GET /api/syllabus
Returns full JSON list of all syllabus entries
---------------------------------
*/
app.get("/api/syllabus", (req, res) => {
  const child = spawn(".\\syllabus_api.exe", ["list"]);

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", chunk => (output += chunk.toString()));
  child.stderr.on("data", chunk => (errorOutput += chunk.toString()));

  child.on("close", () => {
    if (errorOutput && !output) {
      return res.status(500).json({
        error: "C backend error (syllabus list)",
        details: errorOutput
      });
    }

    try {
      const json = JSON.parse(output || "[]");
      res.json(json);
    } catch (err) {
      res.status(500).json({
        error: "Invalid JSON from syllabus backend",
        raw: output
      });
    }
  });
});


/* 
---------------------------------
POST /api/syllabus
Body: { subject, units }
---------------------------------
*/
app.post("/api/syllabus", (req, res) => {
  const { subject, units } = req.body;

  if (!subject || !units) {
    return res.status(400).json({ success: false, message: "Missing required fields" });
  }

  const child = spawn(".\\syllabus_api.exe", ["add"]);
  child.stdin.write(`${subject}|${units}\n`);
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", data => (output += data.toString()));
  child.stderr.on("data", data => (errorOutput += data.toString()));

  child.on("close", () => {
    if (errorOutput && !output) {
      return res.status(500).json({
        success: false,
        message: "C backend error (syllabus add)",
        details: errorOutput
      });
    }

    try {
      const json = JSON.parse(output);
      res.json(json);
    } catch {
      res.status(500).json({
        success: false,
        message: "Invalid JSON from syllabus backend",
        raw: output
      });
    }
  });
});


/*
---------------------------------
DELETE /api/syllabus/:subject
---------------------------------
*/
app.delete("/api/syllabus/:subject", (req, res) => {
  const { subject } = req.params;

  const child = spawn(".\\syllabus_api.exe", ["delete"]);
  child.stdin.write(subject + "\n");
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", data => (output += data.toString()));
  child.stderr.on("data", data => (errorOutput += data.toString()));

  child.on("close", () => {
    if (errorOutput && !output) {
      return res.status(500).json({
        success: false,
        message: "C backend error (syllabus delete)",
        details: errorOutput
      });
    }

    try {
      const json = JSON.parse(output);
      res.json(json);
    } catch {
      res.status(500).json({
        success: false,
        message: "Invalid JSON from C backend (syllabus delete)",
        raw: output
      });
    }
  });
});


/*
---------------------------------
POST /api/syllabus/book
Body: { subject, title, author }
---------------------------------
*/
app.post("/api/syllabus/book", (req, res) => {
  const { subject, title, author } = req.body;

  if (!subject || !title || !author) {
    return res.status(400).json({
      success: false,
      message: "Missing required fields"
    });
  }

  const child = spawn(".\\syllabus_api.exe", ["addBook"]);
  child.stdin.write(`${subject}|${title}|${author}\n`);
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", data => (output += data.toString()));
  child.stderr.on("data", data => (errorOutput += data.toString()));

  child.on("close", () => {
    if (errorOutput && !output) {
      return res.status(500).json({
        success: false,
        message: "C backend error (add book)",
        details: errorOutput
      });
    }

    try {
      const json = JSON.parse(output);
      res.json(json);
    } catch {
      res.status(500).json({
        success: false,
        message: "Invalid JSON from syllabus backend (addBook)",
        raw: output
      });
    }
  });
});


/*
---------------------------------
GET /api/syllabus/:subject
Fetch a single subject
---------------------------------
*/
app.get("/api/syllabus/:subject", (req, res) => {
  const { subject } = req.params;

  const child = spawn(".\\syllabus_api.exe", ["get"]);
  child.stdin.write(subject + "\n");
  child.stdin.end();

  let output = "";
  let errorOutput = "";

  child.stdout.on("data", data => (output += data.toString()));
  child.stderr.on("data", data => (errorOutput += data.toString()));

  child.on("close", () => {
    if (errorOutput && !output) {
      return res.status(500).json({
        error: "C backend error (syllabus get)",
        details: errorOutput
      });
    }

    try {
      const json = JSON.parse(output);

      if (json === NULL || json === null) {
        return res.status(404).json({ error: "Subject not found" });
      }

      res.json(json);
    } catch {
      res.status(500).json({
        error: "Invalid JSON from syllabus backend (get)",
        raw: output
      });
    }
  });
});

app.listen(PORT, () => {
  console.log(`API server running at http://localhost:${PORT}`);
});
app.get("/", (req, res) => {
    res.send("Backend running!");});
    // 🟣 NOTES TEMP STORAGE
let notes = [
    { title: "Sample Note", subject: "DSA", content: "Sample content", faculty: "Prof X" }
];

// 🟢 GET ALL NOTES
app.get("/notes", (req, res) => {
    res.json(notes);
});

// 🟠 ADD NEW NOTE
app.post("/notes", (req, res) => {
    const { title, subject, content, faculty } = req.body;

    if (!title || !subject || !content || !faculty) {
        return res.json({ success: false, message: "All fields required" });
    }

    notes.push({ title, subject, content, faculty });
    res.json({ success: true, message: "Note added!" });
});
