const PLACES = [
  { id: "gate", name: "Main Gate", type: "Entrance" },
  { id: "admin", name: "Administration", type: "Office" },
  { id: "library", name: "University Library", type: "Library" },
  { id: "cs", name: "Computer Science Building", type: "Academic" },
  { id: "eng", name: "Engineering Building", type: "Academic" },
  { id: "cafeteria", name: "Cafeteria", type: "Food" },
  { id: "clinic", name: "Medical Clinic", type: "Service" },
  { id: "sports", name: "Sports Hall", type: "Sports" },
];

const ROUTES = {
  "gate->library": ["Walk straight for 200m", "Turn right at the fountain", "Library is on your left"],
  "gate->cs": ["Walk straight for 150m", "Turn left after Administration", "CS Building is ahead"],
  "library->cs": ["Exit the Library", "Follow the main path for 120m", "CS Building is on your right"],
};

function normalize(s) {
  return (s || "").toLowerCase().trim();
}

function setFeedback(text) {
  document.getElementById("feedback").textContent = text;
}

function renderResults(query) {
  const ul = document.getElementById("results");
  ul.innerHTML = "";

  const q = normalize(query);
  const results = q
    ? PLACES.filter((p) => normalize(p.name).includes(q) || normalize(p.type).includes(q))
    : PLACES.slice(0, 6);

  if (results.length === 0) {
    const li = document.createElement("li");
    li.textContent = "No results";
    ul.appendChild(li);
    return;
  }

  for (const place of results) {
    const li = document.createElement("li");
    const left = document.createElement("span");
    left.textContent = place.name;
    const right = document.createElement("span");
    right.className = "tag";
    right.textContent = place.type;
    li.appendChild(left);
    li.appendChild(right);
    ul.appendChild(li);
  }
}

function populateSelects() {
  const from = document.getElementById("from");
  const to = document.getElementById("to");
  from.innerHTML = "";
  to.innerHTML = "";
  for (const p of PLACES) {
    const opt1 = document.createElement("option");
    opt1.value = p.id;
    opt1.textContent = p.name;
    from.appendChild(opt1);

    const opt2 = document.createElement("option");
    opt2.value = p.id;
    opt2.textContent = p.name;
    to.appendChild(opt2);
  }

  from.value = "gate";
  to.value = "library";
}

function showRoute() {
  const from = document.getElementById("from").value;
  const to = document.getElementById("to").value;
  const stepsEl = document.getElementById("steps");
  stepsEl.innerHTML = "";

  if (from === to) {
    setFeedback("You are already at your destination.");
    return;
  }

  const key = `${from}->${to}`;
  const steps = ROUTES[key] || ["Walk to the main path", "Follow signage toward your destination", "Arrive safely"];
  setFeedback(`Showing directions (${steps.length} steps).`);

  for (const s of steps) {
    const li = document.createElement("li");
    li.textContent = s;
    stepsEl.appendChild(li);
  }
}

document.addEventListener("DOMContentLoaded", () => {
  populateSelects();
  renderResults("");
  setFeedback("Ready.");

  const q = document.getElementById("q");
  q.addEventListener("input", () => renderResults(q.value));
  q.addEventListener("keydown", (e) => {
    if (e.key === "Enter") {
      renderResults(q.value);
      setFeedback("Results updated.");
    }
  });

  document.getElementById("routeBtn").addEventListener("click", showRoute);
});

