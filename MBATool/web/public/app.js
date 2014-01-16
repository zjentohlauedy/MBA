window.App = Ember.Application.create();

var App = window.App

App._constants = {
    TEAM_MAX_PITCHERS:          11,
    TEAM_MAX_BATTERS:           16,
    ROSTER_CUT_TARGET_PITCHERS:  3,
    ROSTER_CUT_TARGET_BATTERS:   5
};

App._progress = {
    stage: 0,
    stages: [
        { number: "1", name: "Start New Season", isDone: false, route: "start-season"  },
        { number: "2", name: "Roster Cut",       isDone: false, route: "roster-cut"    },
        { number: "3", name: "Rookie Draft",     isDone: false, route: "rookie-draft"  },
        { number: "4", name: "Free Agent Draft", isDone: false, route: "free-agents"   },
        { number: "5", name: "Export Season",    isDone: false, route: "export-season" },
        { number: "6", name: "Import Season",    isDone: false, route: "import-season" }
    ].map(function(entry){
        return Ember.Object.create().setProperties(entry);
    })
};

App._teams = [
    { name: "Team01",
      isSelected: true,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team02",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team03",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team04",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team05",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team06",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team07",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team08",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team09",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team10",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team11",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team12",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team13",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team14",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team15",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team16",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team17",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team18",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team19",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team20",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team21",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team22",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team23",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team24",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team25",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team26",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team27",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team28",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team29",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team30",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team31",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    },
    { name: "Team32",
      isSelected: false,
      isComplete: false,
      isError:    false,
      draftedRookiePitcher: false,
      draftedRookieBatter:  false,
      pitchers: [
          { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isCut: false },
          { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isCut: false },
          { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isCut: false },
          { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isCut: false },
          { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isCut: false },
          { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isCut: false },
          { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isCut: false },
          { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isCut: false },
          { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isCut: false },
          { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      }),
      batters: [
          { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isCut: false },
          { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isCut: false },
          { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isCut: false },
          { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isCut: false },
          { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isCut: false },
          { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isCut: false },
          { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isCut: false },
          { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isCut: false },
          { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isCut: false },
          { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isCut: false },
          { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false },
          { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isCut: false },
          { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isCut: false },
          { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isCut: false }
      ].map(function(entry){
          return Ember.Object.create().setProperties(entry);
      })
    }
].map(function(entry){
    return Ember.Object.create().setProperties(entry);
});

App._rookies = Ember.Object.create().setProperties({
    draftOrder: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
                 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31],
    pitchers: [
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false }
    ].map(function(entry){
        return Ember.Object.create().setProperties(entry);
    }),
    batters: [
        { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isSelected: false },
        { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isSelected: false },
        { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isSelected: false },
        { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isSelected: false },
        { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isSelected: false },
        { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isSelected: false },
        { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isSelected: false },
        { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isSelected: false },
        { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isSelected: false },
        { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isSelected: false },
        { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isSelected: false },
        { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isSelected: false },
        { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isSelected: false },
        { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isSelected: false },
        { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isSelected: false },
        { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isSelected: false },
        { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isSelected: false },
        { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isSelected: false },
        { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isSelected: false },
        { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isSelected: false },
        { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false }
    ].map(function(entry){
        return Ember.Object.create().setProperties(entry);
    })
});

App._freeAgents = Ember.Object.create().setProperties({
    draftOrder: [0,1,2,3,3,2,1,0],
    pitchers: [
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false },
        { name: "Hartman, Dave",      hand: "R", speed: "4", control: "4", bunt: "6", fatigue: "5", record: "0 - 0",  innings:   "5.0", era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3",  isSelected: false },
        { name: "Hearns, Bill",       hand: "R", speed: "6", control: "3", bunt: "5", fatigue: "8", record: "1 - 0",  innings:  "13.0", era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3",  isSelected: false },
        { name: "Michael, Danny",     hand: "R", speed: "5", control: "5", bunt: "4", fatigue: "7", record: "0 - 0",  innings:  "17.1", era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6",  isSelected: false },
        { name: "Montgomery, Curtis", hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "6", record: "5 - 5",  innings:  "60.0", era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1" , isSelected: false },
        { name: "Risc, Paul",         hand: "R", speed: "5", control: "6", bunt: "4", fatigue: "7", record: "3 - 7",  innings:  "96.0", era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19", isSelected: false },
        { name: "Stern, Dan",         hand: "L", speed: "5", control: "6", bunt: "5", fatigue: "5", record: "6 - 1",  innings:  "81.0", era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12", isSelected: false },
        { name: "Thorpe, Denny",      hand: "R", speed: "5", control: "5", bunt: "6", fatigue: "6", record: "1 - 1",  innings:  "21.0", era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9",  isSelected: false },
        { name: "Vowles, Keith",      hand: "L", speed: "5", control: "5", bunt: "7", fatigue: "7", record: "5 - 5",  innings:  "48.2", era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27", isSelected: false },
        { name: "Conley, Bud",        hand: "R", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "2 - 11", innings: "100.1", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58", isSelected: false },
        { name: "Gold, Johnny",       hand: "L", speed: "6", control: "6", bunt: "5", fatigue: "6", record: "3 - 5",  innings: "104.0", era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3",  isSelected: false }
    ].map(function(entry){
        return Ember.Object.create().setProperties(entry);
    }),
    batters: [
        { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isSelected: false },
        { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isSelected: false },
        { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isSelected: false },
        { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isSelected: false },
        { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isSelected: false },
        { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isSelected: false },
        { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isSelected: false },
        { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isSelected: false },
        { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isSelected: false },
        { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isSelected: false },
        { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isSelected: false },
        { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isSelected: false },
        { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isSelected: false },
        { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isSelected: false },
        { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isSelected: false },
        { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isSelected: false },
        { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isSelected: false },
        { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isSelected: false },
        { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isSelected: false },
        { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isSelected: false },
        { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isSelected: false },
        { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isSelected: false },
        { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isSelected: false },
        { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isSelected: false },
        { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isSelected: false },
        { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isSelected: false },
        { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isSelected: false },
        { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isSelected: false },
        { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isSelected: false },
        { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isSelected: false },
        { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isSelected: false },
        { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isSelected: false },
        { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isSelected: false },
        { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isSelected: false },
        { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isSelected: false },
        { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isSelected: false },
        { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isSelected: false },
        { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isSelected: false },
        { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isSelected: false },
        { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isSelected: false },
        { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isSelected: false },
        { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isSelected: false },
        { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isSelected: false },
        { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isSelected: false },
        { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isSelected: false },
        { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isSelected: false },
        { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isSelected: false },
        { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isSelected: false },
        { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isSelected: false },
        { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isSelected: false },
        { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Stamper, Jason",      position: "C",  pos2: "IF", hand: "L", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "2.00", isSelected: false },
        { name: "Ulrich, Ed",          position: "C",  pos2: "1B", hand: "R", power: "7", hitnrun: "5", bunt: "6", running: "5", range: "3", arm: "6", games: "62", avg: ".279", homers: "19", steals: "0", slugging: ".598", oba: ".301", rpg: "1.03", isSelected: false },
        { name: "Bennett, Richard",    position: "1B", pos2: "OF", hand: "R", power: "5", hitnrun: "6", bunt: "5", running: "7", range: "5", arm: "5", games: "62", avg: ".286", homers: "14", steals: "9", slugging: ".514", oba: ".361", rpg: "0.98", isSelected: false },
        { name: "Graysmark, John",     position: "1B", pos2: "IF", hand: "R", power: "5", hitnrun: "8", bunt: "6", running: "7", range: "5", arm: "5", games: "13", avg: ".265", homers: "0",  steals: "1", slugging: ".408", oba: ".294", rpg: "0.69", isSelected: false },
        { name: "Bond, Charles",       position: "2B", pos2: "IF", hand: "R", power: "5", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "16", avg: ".304", homers: "1",  steals: "0", slugging: ".478", oba: ".347", rpg: "0.75", isSelected: false },
        { name: "Thomas, Roy",         position: "2B", pos2: "C",  hand: "L", power: "5", hitnrun: "5", bunt: "5", running: "6", range: "6", arm: "7", games: "52", avg: ".282", homers: "11", steals: "0", slugging: ".518", oba: ".358", rpg: "1.04", isSelected: false },
        { name: "Kaume, Robert",       position: "3B", pos2: "OF", hand: "R", power: "7", hitnrun: "5", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".281", homers: "22", steals: "0", slugging: ".628", oba: ".338", rpg: "1.05", isSelected: false },
        { name: "Brenneys, Marty",     position: "SS", pos2: "IF", hand: "R", power: "6", hitnrun: "5", bunt: "6", running: "5", range: "5", arm: "6", games: "62", avg: ".197", homers: "6",  steals: "1", slugging: ".305", oba: ".255", rpg: "0.50", isSelected: false },
        { name: "Giles, Charles",      position: "SS", pos2: "IF", hand: "L", power: "5", hitnrun: "4", bunt: "5", running: "5", range: "2", arm: "6", games: "52", avg: ".212", homers: "5",  steals: "0", slugging: ".380", oba: ".291", rpg: "0.62", isSelected: false },
        { name: "Fasching, Scott",     position: "LF", pos2: "C",  hand: "L", power: "3", hitnrun: "3", bunt: "6", running: "2", range: "6", arm: "6", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Smith, Guy",          position: "LF", pos2: "OF", hand: "L", power: "6", hitnrun: "5", bunt: "4", running: "4", range: "5", arm: "5", games: "62", avg: ".185", homers: "9",  steals: "0", slugging: ".373", oba: ".243", rpg: "0.68", isSelected: false },
        { name: "Ford, Roy",           position: "CF", pos2: "OF", hand: "R", power: "5", hitnrun: "5", bunt: "5", running: "5", range: "5", arm: "5", games: "0",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Nicholson, Benjamin", position: "CF", pos2: "IF", hand: "R", power: "4", hitnrun: "6", bunt: "4", running: "5", range: "5", arm: "5", games: "62", avg: ".210", homers: "9",  steals: "0", slugging: ".410", oba: ".325", rpg: "0.76", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false },
        { name: "Campbell, Colin",     position: "RF", pos2: "C",  hand: "L", power: "5", hitnrun: "6", bunt: "6", running: "7", range: "6", arm: "6", games: "62", avg: ".226", homers: "12", steals: "2", slugging: ".481", oba: ".348", rpg: "0.98", isSelected: false },
        { name: "Mooney, Robert",      position: "RF", pos2: "OF", hand: "R", power: "4", hitnrun: "6", bunt: "5", running: "5", range: "4", arm: "5", games: "1",  avg: ".000", homers: "0",  steals: "0", slugging: ".000", oba: ".000", rpg: "0.00", isSelected: false }
    ].map(function(entry){
        return Ember.Object.create().setProperties(entry);
    })
});

App.Router.map(function() {
    this.route("start-season",    { path: "/start"  });
    this.route("roster-cut",      { path: "/cut"    });
    this.route("rookie-draft",    { path: "/rookie" });
    this.route("free-agents",     { path: "/agents" });
    this.route("export-season",   { path: "/export" });
    this.route("import-season",   { path: "/import" });
    this.route("season-complete", { path: "/done"   });
});

App.IndexRoute = Ember.Route.extend({
  redirect: function() {
    this.transitionTo(App._progress.stages[App._progress.stage].route);
  }
});

App.ProgressController = Ember.ObjectController.extend({
    stage:  App._progress.stage,
    stages: App._progress.stages,
    actions: {
        nextStage: function() {
            if (this.stage >= this.stages.length) {
                this.stages.setEach("isDone", false);
                this.stage = 0;

                this.transitionToRoute(this.stages[this.stage].route);

                return;
            }

            this.stages[this.stage].set("isDone", true);
            this.stage++;

            if (this.stage < this.stages.length) {
                this.transitionToRoute(this.stages[this.stage].route);
            }
            else {
                this.transitionToRoute("season-complete");
            }
        }
    }
});

App.RosterCutController = Ember.ObjectController.extend({
    needs:          "progress",
    teams:          App._teams,
    currentTeam:    App._teams[0],
    pitchers: {
        isComplete: false,
        isError:    false
    },
    batters: {
        isComplete: false,
        isError:    false
    },
    stageComplete:  false,
    actions: {
        selectTeam: function(team) {
            if (team.isSelected) return;

            team.set("isSelected", true);
            this.currentTeam.set("isSelected", false);
            this.set( "currentTeam", team );

            this.send("updatePitchersStatus");
            this.send("updateBattersStatus");
        },
        toggleCutPitcher: function(pitcher) {
            pitcher.set("isCut", !pitcher.isCut);

            this.send("updatePitchersStatus");
            this.send("updateTeamStatus");
        },
        toggleCutBatter: function(batter) {
            batter.set("isCut", !batter.isCut);

            this.send("updateBattersStatus");
            this.send("updateTeamStatus");
        },
        updatePitchersStatus: function() {
            var count = this.currentTeam.pitchers.reduce(function(value, pitcher){ return (pitcher.isCut) ? value : value + 1; }, 0);

            if      (count > App._constants.ROSTER_CUT_TARGET_PITCHERS) { this.set("pitchers.isComplete", false); this.set("pitchers.isError", false); }
            else if (count < App._constants.ROSTER_CUT_TARGET_PITCHERS) { this.set("pitchers.isComplete", false); this.set("pitchers.isError", true ); }
            else                                                        { this.set("pitchers.isComplete", true ); this.set("pitchers.isError", false); }
        },
        updateBattersStatus: function() {
            var count = this.currentTeam.batters.reduce(function(value, batter){ return (batter.isCut) ? value : value + 1; }, 0);

            if      (count > App._constants.ROSTER_CUT_TARGET_BATTERS) { this.set("batters.isComplete", false); this.set("batters.isError", false); }
            else if (count < App._constants.ROSTER_CUT_TARGET_BATTERS) { this.set("batters.isComplete", false); this.set("batters.isError", true ); }
            else                                                       { this.set("batters.isComplete", true ); this.set("batters.isError", false); }
        },
        updateTeamStatus: function() {
            if   (this.pitchers.isComplete && this.batters.isComplete) this.currentTeam.set("isComplete", true );
            else                                                       this.currentTeam.set("isComplete", false);

            if   (this.pitchers.isError || this.batters.isError) this.currentTeam.set("isError", true );
            else                                                 this.currentTeam.set("isError", false);

            this.send("updateStatus");
        },
        updateStatus: function() {
            var count = this.teams.reduce(function(value, team){ return (team.isComplete) ? value : value + 1; }, 0);

            if   (count > 0) this.set("stageComplete", false);
            else             this.set("stageComplete", true );
        },
        finishStage: function() {
            if (this.stageComplete) {
                this.get("controllers.progress").send('nextStage');
            }
        }
    }
});

App.RookieDraftController = Ember.ObjectController.extend({
    needs:                   "progress",
    team:                    App._teams[App._rookies.draftOrder[0]],
    rookies:                 App._rookies,
    currentTeamIdx:          0,
    currentPitcherSortField: "",
    currentBatterSortField:  "",
    toggleRookieButtonLabel: "Show Batters",
    showRookiePitchers:      true,
    showRookieBatters:       false,
    canDraft:                true,
    stageComplete:           false,
    actions: {
        toggleRookieTable: function() {
            if (this.showRookiePitchers) {
                this.set("showRookiePitchers", false);
                this.set("showRookieBatters",  true );

                this.set("toggleRookieButtonLabel", "Show Pitchers");

                this.rookies.pitchers.setEach("isSelected", false);
            }
            else {
                this.set("showRookiePitchers", true );
                this.set("showRookieBatters",  false);

                this.set("toggleRookieButtonLabel", "Show Batters");

                this.rookies.batters.setEach("isSelected", false);
            }

            this.send("setDraftStatus");
        },
        sortPitchers: function(field) {
            if (field === this.currentPitcherSortField) {
                this.rookies.set("pitchers", this.rookies.pitchers.reverseObjects());
                return;
            }

            this.set("currentPitcherSortField", field);

            this.rookies.set("pitchers", this.rookies.pitchers.sortBy(field));
        },
        sortBatters: function(field) {
            if (field === this.currentBatterSortField) {
                this.rookies.set("batters", this.rookies.batters.reverseObjects());
                return;
            }

            this.set("currentBatterSortField", field);

            this.rookies.set("batters", this.rookies.batters.sortBy(field));
        },
        selectRookiePitcher: function(pitcher) {
            if (pitcher.isSelected) {
                pitcher.set("isSelected", false);
                return;
            }

            this.rookies.pitchers.setEach("isSelected", false);

            pitcher.set("isSelected", true);
        },
        selectRookieBatter: function(batter) {
            if (batter.isSelected) {
                batter.set("isSelected", false);
                return;
            }

            this.rookies.batters.setEach("isSelected", false);

            batter.set("isSelected", true);
        },
        draftRookie: function() {
            if (!this.canDraft) return;

            var pitcher;
            if (pitcher = this.rookies.pitchers.findBy("isSelected")) {
                this.rookies.pitchers.removeObject(pitcher);
                this.team.   pitchers.   addObject(pitcher);
                this.team.set("draftedRookiePitcher", true);
                this.send("showNextTeam");
                return;
            }

            var batter;
            if (batter = this.rookies.batters.findBy("isSelected")) {
                this.rookies.batters.removeObject(batter);
                this.team   .batters.   addObject(batter);
                this.team.set("draftedRookieBatter", true);
                this.send("showNextTeam");
                return;
            }
        },
        showNextTeam: function() {
            var nextTeamIdx = this.currentTeamIdx + 1;

            if (nextTeamIdx == App._rookies.draftOrder.length) {
                this.set("canDraft",      false);
                this.set("stageComplete", true );
                return;
            }

            this.set("currentTeamIdx", nextTeamIdx);
            this.set("team", App._teams[App._rookies.draftOrder[nextTeamIdx]]);

            this.send("setDraftStatus");
        },
        setDraftStatus: function() {
            if (this.stageComplete) return;

            if (this.showRookiePitchers && this.team.draftedRookiePitcher) {
                this.set("canDraft", false);
                return;
            }

            if (this.showRookieBatters && this.team.draftedRookieBatter) {
                this.set("canDraft", false);
                return;
            }

            this.set("canDraft", true);
        },
        finishStage: function() {
            if (this.stageComplete) {
                this.get("controllers.progress").send('nextStage');
            }
        }
    }
});

App.FreeAgentsController = Ember.ObjectController.extend({
    needs:                      "progress",
    team:                       App._teams[0],
    freeAgents:                 App._freeAgents,
    currentTeamIdx:             0,
    currentPitcherSortField:    "",
    currentBatterSortField:     "",
    toggleFreeAgentButtonLabel: "Show Batters",
    showFreeAgentPitchers:      true,
    showFreeAgentBatters:       false,
    canDraft:                   true,
    stageComplete:              false,
    actions: {
        toggleFreeAgentTable: function() {
            if (this.showFreeAgentPitchers) {
                this.set("showFreeAgentPitchers", false);
                this.set("showFreeAgentBatters",  true );

                this.set("toggleFreeAgentButtonLabel", "Show Pitchers");

                this.freeAgents.pitchers.setEach("isSelected", false);
            }
            else {
                this.set("showFreeAgentPitchers", true );
                this.set("showFreeAgentBatters",  false);

                this.set("toggleFreeAgentButtonLabel", "Show Batters");

                this.freeAgents.batters.setEach("isSelected", false);
            }

            this.send("setDraftStatus");
        },
        sortPitchers: function(field) {
            if (field === this.currentPitcherSortField) {
                this.freeAgents.set("pitchers", this.freeAgents.pitchers.reverseObjects());
                return;
            }

            this.set("currentPitcherSortField", field);

            this.freeAgents.set("pitchers", this.freeAgents.pitchers.sortBy(field));
        },
        sortBatters: function(field) {
            if (field === this.currentBatterSortField) {
                this.freeAgents.set("batters", this.freeAgents.batters.reverseObjects());
                return;
            }

            this.set("currentBatterSortField", field);

            this.freeAgents.set("batters", this.freeAgents.batters.sortBy(field));
        },
        selectFreeAgentPitcher: function(pitcher) {
            if (pitcher.isSelected) {
                pitcher.set("isSelected", false);
                return;
            }

            this.freeAgents.pitchers.setEach("isSelected", false);

            pitcher.set("isSelected", true);
        },
        selectFreeAgentBatter: function(batter) {
            if (batter.isSelected) {
                batter.set("isSelected", false);
                return;
            }

            this.freeAgents.batters.setEach("isSelected", false);

            batter.set("isSelected", true);
        },
        draftFreeAgent: function() {
            if (!this.canDraft) return;

            var pitcher;
            if (pitcher = this.freeAgents.pitchers.findBy("isSelected")) {
                this.freeAgents.pitchers.removeObject(pitcher);
                this.team.      pitchers.   addObject(pitcher);
                this.send("showNextTeam");
                return;
            }

            var batter;
            if (batter = this.freeAgents.batters.findBy("isSelected")) {
                this.freeAgents.batters.removeObject(batter);
                this.team      .batters.   addObject(batter);
                this.send("showNextTeam");
                return;
            }
        },
        showNextTeam: function() {
            var nextTeamIdx = this.currentTeamIdx + 1;

            if (nextTeamIdx == App._freeAgents.draftOrder.length) {
                this.set("canDraft",      false);
                this.set("stageComplete", true );
                return;
            }

            this.set("currentTeamIdx", nextTeamIdx);
            this.set("team", App._teams[App._freeAgents.draftOrder[nextTeamIdx]]);

            this.send("setDraftStatus");
        },
        setDraftStatus: function() {
            if (this.stageComplete) return;

            if (this.showFreeAgentPitchers && (this.team.pitchers.length >= App._constants.TEAM_MAX_PITCHERS)) {
                this.set("canDraft", false);
                return;
            }

            if (this.showFreeAgentBatters && (this.team.batters.length >= App._constants.TEAM_MAX_BATTERS)) {
                this.set("canDraft", false);
                return;
            }

            this.set("canDraft", true);
        },
        finishStage: function() {
            if (this.stageComplete) {
                this.get("controllers.progress").send('nextStage');
            }
        }
    }
});

App.NextButtonController = Ember.ObjectController.extend({
    needs: ['progress']
});

App.NextButtonView = Ember.View.extend({
    templateName: "next-button",
    name: "Next",
    click: function(event) {
        this.get("controller.controllers.progress").send('nextStage');
    }
});
