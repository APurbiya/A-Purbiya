// Set the document dimensions
setDocDimensions(125, 125);

// Store final lines here
const finalLines = [];

// Maze parameters
const cellSize = 5; // Size of each cell in the maze
const rows = Math.floor(125 / cellSize);
const cols = Math.floor(125 / cellSize);

// Create a grid of cells
const grid = Array(rows).fill(null).map(() => Array(cols).fill(false));

// Directions for moving in the grid: right, down, left, up
const directions = [
  [1, 0],
  [0, 1],
  [-1, 0],
  [0, -1]
];

// Helper function to shuffle directions using bt.randInRange
function shuffle(array) {
  for (let i = array.length - 1; i > 0; i--) {
    const j = bt.randIntInRange(0, i);
    [array[i], array[j]] = [array[j], array[i]];
  }
}

// Recursive backtracking function to carve the maze
function carve(x, y) {
  grid[y][x] = true; // Mark the current cell as visited
  shuffle(directions); // Shuffle the directions

  for (const [dx, dy] of directions) {
    const nx = x + dx;
    const ny = y + dy;
    const mx = x + 2 * dx;
    const my = y + 2 * dy;

    if (mx >= 0 && my >= 0 && mx < cols && my < rows && !grid[my][mx]) {
      grid[ny][nx] = true;
      carve(mx, my);
    }
  }
}

// Start carving from the top-left corner
carve(0, 0);

// Draw the maze using a Turtle
const myTurtle = new bt.Turtle();
myTurtle.up();

// Function to draw walls
function drawWall(x1, y1, x2, y2) {
  myTurtle.jump([x1, y1]);
  myTurtle.down();
  myTurtle.goTo([x2, y2]);
  myTurtle.up();
}

// Function to draw a cell's borders
function drawCell(x, y) {
  const x1 = x * cellSize;
  const y1 = y * cellSize;
  const x2 = (x + 1) * cellSize;
  const y2 = (y + 1) * cellSize;

  drawWall(x1, y1, x2, y1); // Top wall
  drawWall(x2, y1, x2, y2); // Right wall
  drawWall(x2, y2, x1, y2); // Bottom wall
  drawWall(x1, y2, x1, y1); // Left wall
}

// Draw borders with openings for start and end
function drawBorders() {
  // Draw top border with opening at (1, 0)
  for (let x = 0; x < cols; x++) {
    if (x !== Math.floor(cols / 2)) {
      drawCell(x, 0);
    }
  }
  // Draw right border
  for (let y = 0; y < rows; y++) {
    drawCell(cols - 1, y);
  }
  // Draw bottom border with opening at (cols-1, rows)
  for (let x = 0; x < cols; x++) {
    if (x !== Math.floor(cols / 2)) {
      drawCell(x, rows - 1);
    }
  }
  // Draw left border
  for (let y = 0; y < rows; y++) {
    drawCell(0, y);
  }
}

// Call the function to draw borders
drawBorders();

// Iterate through the grid and draw the maze walls
for (let y = 0; y < rows; y++) {
  for (let x = 0; x < cols; x++) {
    const x1 = x * cellSize;
    const y1 = y * cellSize;
    const x2 = (x + 1) * cellSize;
    const y2 = (y + 1) * cellSize;

    if (!grid[y][x]) {
      if (!(y === 0 && x === 0)) drawWall(x1, y1, x2, y1); // Top wall
      if (!(y === rows - 1 && x === cols - 1)) drawWall(x2, y1, x2, y2); // Right wall
      drawWall(x2, y2, x1, y2); // Bottom wall
      drawWall(x1, y2, x1, y1); // Left wall
    } else {
      if (x < cols - 1 && !grid[y][x + 1]) {
        drawWall(x2, y1, x2, y2); // Right wall
      }
      if (y < rows - 1 && !grid[y + 1][x]) {
        drawWall(x1, y2, x2, y2); // Bottom wall
      }
    }
  }
}

// Get the turtle's path and add it to the final lines
const path = myTurtle.lines();
finalLines.push(...path);

// Draw the final maze
drawLines(finalLines);
