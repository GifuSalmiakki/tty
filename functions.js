
function findTech(techs, toBeFound) {


  for (var i = 0; i < techs.length; ++i) {
    if (techs[i] === toBeFound) {
      if (techs[i][0] == toBeFound) {
        return i;
      }
    }
  }
}

/**
 * Build yearly technology stats
 *
 * @param {object} techStats StackOverfow stats
 * @returns {object} Year by year stats of technologies mentioned in StackOverflow
 */
function buildYearlyTechStats(techStats) {
  var currentTechs = {};

  for (const year in techStats) {
    techs = Object.keys(techStats[year])
    currents = {};
    // gather all current technologies to a new object

    for (var i = 0; i < techs.length; ++i) {
      if (techs[i].startsWith("current")) {
        for (const [key, value] of Object.entries(techStats[year][techs[i]]))
        // access the {'tech': mentions} -pairs, and copy them to a new object
        currents[key] = value;          
      }
    }
    currentTechs[year] = currents;
  }
  return currentTechs;
  
}

/**
 * Update table contents
 *
 * @param {HTMLTableElement} table DOM element for the table
 * @param {object} yearlyTechStats Year by year stats of technologies mentioned in StackOverflow
 * @param {Array<string>} selectedTechs Technologies selected
 * @param {number} firstYear First year of data selected
 * @param {number} lastYear Last year of data selected
 */
function updateTable(table, yearlyTechStats, selectedTechs, firstYear, lastYear) {

  var headings = ["Technology"];
  for (const year in yearlyTechStats){
    if (year >= firstYear && year <= lastYear){
      headings.push(year);
    }
  }
  techs = buildRowData(yearlyTechStats, selectedTechs, firstYear, lastYear);
  table.tHead.innerHTML = constructTableHeadHtml(headings);
  tbody = document.querySelector("tbody");
  tbody.innerHTML = constructTableRowsHtml(techs);

}

/**
 * Build row data to be shown in a table
 *
 * @param {object} yearlyTechStats Year by year stats of technologies mentioned in StackOverflow
 * @param {Array<string>} selectedTechs Technologies selected
 * @param {number} firstYear First year of data selected
 * @param {number} lastYear Last year of data selected
 * @returns {Array<string|number>}
 */
 function buildRowData(yearlyTechStats, selectedTechs, firstYear, lastYear) {

  var foundTechs = [];

  for (const year in yearlyTechStats) {
    const yearlyTechs = Object.keys(yearlyTechStats[year])

    if (year >= firstYear && year <= lastYear) {
      // check if whithin wanted year-range  

      for (var i = 0; i < selectedTechs.length; ++i) {
        tech = selectedTechs[i];
        const index = findTech(foundTechs, tech);

        if (yearlyTechs.indexOf(tech) != -1 && index == undefined) {
          // tech exists, is not yet in table
          const newTableRow = [tech, yearlyTechStats[year][tech]];
          foundTechs.push(newTableRow);
        }
        else if (yearlyTechs.indexOf(tech) == -1 && index == undefined) {

          // tech does not exist, is not yet in table
          const newTableRow = [tech, 0];
          foundTechs.push(newTableRow);
        }
        else if (yearlyTechs.indexOf(tech) != -1 && index != undefined) {
          // tech exists, is in table
          foundTechs[index].push(yearlyTechStats[year][tech]);
        }
        else {
          // tech does not exist, is in table
          foundTechs[index].push(0);
        }
      }
    }
  } return foundTechs;
}

/**
 * Get HTML of table rows
 *
 * @param {Array<string|number>} rowData
 * @returns {string} HTML of the table rows
 */
function constructTableRowsHtml(rowData) {

  var string = '';

  for (var i = 0; i < rowData.length; ++i) {
    for (var j = 0; j < rowData[i].length; ++j) {

      if (j == 0) {
        // first row starts table definition
        string += `<tr><td>${rowData[i][j]}</td>`;
      }
      else if (j == rowData[i].length - 1) {
        // last row finishes table definition
        string += `<td>${rowData[i][j]}</td></tr>`
      }
      else {
        // middle rows
        string += `<td>${rowData[i][j]}</td>`;
      }
    }
  } return string;
}

/**
 * Get HTML of table heading row
 *
 * @param {Array<string|number>} headings Table headings
 * @returns {string} HTML of the heading row
 */
function constructTableHeadHtml(headings) {
  var string = '';

  for (var i = 0; i < headings.length; ++i) {
    if (i == 0) {
      // start row definition
      string += `<tr><th>${headings[i]}</th>`;
    }
    else if (i == headings.length - 1) {
      // end row definition
      string += `<th>${headings[i]}</th></tr>`;
    }
    else {
      string += `<th>${headings[i]}</th>`;
    }
  }
  return string;
}
