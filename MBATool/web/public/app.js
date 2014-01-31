window.App = Ember.Application.create();

var App = window.App

App._constants = {
/* TEMP
    TEAM_MAX_PITCHERS:          10,
    TEAM_MAX_BATTERS:           15,
    ROSTER_CUT_TARGET_PITCHERS:  3,
    ROSTER_CUT_TARGET_BATTERS:   5
*/
    TEAM_MAX_PITCHERS:          15,
    TEAM_MAX_BATTERS:           20,
    ROSTER_CUT_TARGET_PITCHERS: 10,
    ROSTER_CUT_TARGET_BATTERS:  15
};

App._utils = {
    decorateTeams: function(teams) {

        for ( var i = 0; i < teams.length; i++ ) {
            teams[i].pitchers             = [];
            teams[i].batters              = [];
            teams[i].isSelected           = false;
            teams[i].isComplete           = false;
            teams[i].isError              = false;
            teams[i].draftedRookiePitcher = false;
            teams[i].draftedRookieBatter  = false;
        }

        return teams.map(function(entry){
            return Ember.Object.create().setProperties(entry);
        });
    },
    decoratePitcher: function(player, stats) {
        innings = stats.innings + (stats.outs / 10);
        at_bats = (stats.innings * 3) + stats.outs + stats.walks + stats.hits;
        era     = (innings == 0) ? 0 : stats.earned_runs / (innings / 9);
        vsba    = (at_bats == 0) ? 0 : stats.hits / at_bats;
        ipg     = (stats.games == 0) ? 0 : innings / stats.games;
        whip    = (innings == 0) ? 0 : (stats.walks + stats.hits) / innings;
        sop9    = (innings == 0) ? 0 : stats.strike_outs / (innings / 9);
        eff     = (stats.innings - stats.hits) + (stats.strike_outs - stats.hits);

        player.isCut   = false;
        player.record  = stats.wins + " - " + stats.losses;
        player.innings = innings.toFixed(1);
        player.era     = era.toFixed(2);
        player.vsba    = vsba.toFixed(3).replace(/^0\./,".");
        player.ipg     = ipg.toFixed(2);
        player.whip    = whip.toFixed(3);
        player.sop9    = sop9.toFixed(2);
        player.eff     = (eff > 0) ? "+" + eff : eff;

        return Ember.Object.create().setProperties(player);
    },
    decorateBatter: function(player, stats) {
        avg      = (stats.at_bats == 0) ? 0 : stats.hits / stats.at_bats;
        slugging = (stats.at_bats == 0) ? 0 : (stats.hits + (2 * stats.doubles) + (3 * stats.triples) + (4 *stats.home_runs)) / stats.at_bats;
        oba      = (stats.at_bats == 0) ? 0 : (stats.hits + stats.walks) / stats.at_bats;
        rpg      = (stats.games   == 0) ? 0 : (stats.runs + stats.runs_batted_in - stats.home_runs) / stats.games;

        player.isCut    = false;
        player.games    = stats.games;
        player.avg      = avg.toFixed(3).replace(/^0\./,".");
        player.homers   = stats.home_runs;
        player.steals   = stats.steals;
        player.slugging = slugging.toFixed(3).replace(/^0\./,".");
        player.oba      = oba.toFixed(3).replace(/^0\./,".");
        player.rpg      = rpg.toFixed(2);

        return Ember.Object.create().setProperties(player);
    },
    findLink: function(links, rel) {

        for (var i = 0; i < links.length; i++) {
            if (links[i].rel === rel) {
                return links[i].href;
            }
        }

        return null;
    }
}

App._progress = {
    stage: 0,
    stages: [
        { number: "1", name: "Start New Season", status: "progress-curr", route: "start-season"  },
        { number: "2", name: "Roster Cut",       status: "progress-todo", route: "roster-cut"    },
        { number: "3", name: "Rookie Draft",     status: "progress-todo", route: "rookie-draft"  },
        { number: "4", name: "Free Agent Draft", status: "progress-todo", route: "free-agents"   },
        { number: "5", name: "Export Season",    status: "progress-todo", route: "export-season" },
        { number: "6", name: "Import Season",    status: "progress-todo", route: "import-season" }
    ].map(function(entry){
        return Ember.Object.create().setProperties(entry);
    })
};

App._teams = [];

App._rookies = Ember.Object.create().setProperties({
    draftOrder: [],
    pitchers:   [],
    batters:    []
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
                this.stages.setEach("status", "progress-todo");
                this.stage = 0;

                this.transitionToRoute(this.stages[this.stage].route);

                this.stages[this.stage].set("status", "progress-curr");

                return;
            }

            this.stages[this.stage].set("status", "progress-done");
            this.stage++;

            if (this.stage < this.stages.length) {
                this.transitionToRoute(this.stages[this.stage].route);

                this.stages[this.stage].set("status", "progress-curr");
            }
            else {
                this.transitionToRoute("season-complete");
            }
        }
    }
});

App.StartSeasonController = Ember.ObjectController.extend({
    needs:   'progress',
    actions: {
        startNewSeason: function() {
            var controller = this;
            $.ajax( "/mba/actions/start_season", {
                type: 'POST',
                success: function(data) {
                    App._season = data.Season;
                    controller.send('loadTeams');
                },
                error: function() {
                    alert("Error starting new season!");
                }
            });
        },
        loadTeams: function() {
            var controller = this;
            $.ajax( "/mba/resources/teams", {
                success: function(teams) {
                    App._teams.setObjects(App._utils.decorateTeams(teams));
                    controller.send('finishStage');
                },
                error: function() {
                    alert("Error loading teams!")
                }
            });
        },
        finishStage: function() {
            this.get("controllers.progress").send('nextStage');
        }
    }
});


var loadPlayer = function(team, player) {
    var deferred = $.Deferred();

    $.ajax( App._utils.findLink( player.links, "self" ), {
        success: function(playerDetails) {
            $.ajax( App._utils.findLink( playerDetails.links, "stats" ) + "?season=" + (App._season - 1) + "&phase=1", {
                success: function(data) {

                    if (playerDetails.player_type == 1) { team.pitchers.addObject(App._utils.decoratePitcher(playerDetails, data[0])); }
                    if (playerDetails.player_type == 2) { team.batters. addObject(App._utils.decorateBatter( playerDetails, data[0])); }

                    deferred.resolve();
                },
                error: function() {
                    alert("Error loading player stats!");
                    deferred.reject();
                }
            });
        },
        error: function() {
            alert("Error loading player!");
            deferred.reject();
        }
    });

    return deferred.promise();
}


var loadRookie = function(rookie) {
    var deferred = $.Deferred();

    $.ajax( App._utils.findLink( rookie.links, "self" ), {
        success: function(rookieDetails) {

            rookieDetails.isSelected = false;

            if (rookieDetails.player_type == 1) { App._rookies.pitchers.addObject(Ember.Object.create().setProperties(rookieDetails)); }
            if (rookieDetails.player_type == 2) { App._rookies.batters. addObject(Ember.Object.create().setProperties(rookieDetails)); }

            deferred.resolve();
        },
        error: function() {
            alert("Error loading rookie!");
            deferred.reject();
        }
    });

    return deferred.promise();
}

App.RosterCutController = Ember.ObjectController.extend({
    needs:          ["progress", "rookie-draft"],
    teams:          App._teams,
    currentTeam:    Ember.Object.create(),
    pitchers: {
        isComplete: false,
        isError:    false
    },
    batters: {
        isComplete: false,
        isError:    false
    },
// TEMP
//    stageComplete:  false,
    stageComplete:  true,
    actions: {
        selectTeam: function(team) {
            if (team.isSelected) return;

            team.set("isSelected", true);
            this.currentTeam.set("isSelected", false);
            this.set( "currentTeam", team );


            if ( team.pitchers.length == 0 ) {
                var controller = this;

                $.ajax( "/mba/resources/teams/" + team.team_id + "/players?season=" + App._season + "&phase=1", {
                    success: function(players) {
                        controller.send('loadPlayers', team, players );
                    },
                    error: function() {
                        alert("Error loading players!")
                    }
                });
            }
            else
            {
                this.send("updatePitchersStatus");
                this.send("updateBattersStatus");
            }
        },
        loadPlayers: function(team, players) {
            var promises = [];
            var controller = this;

            for (var i = 0; i < players.length; i++) {
                promises.push( loadPlayer(team, players[i]) );
            }

            $.when.apply(null, promises).done(function() {
                console.log("all done");
                controller.send("updatePitchersStatus");
                controller.send("updateBattersStatus");
                controller.send("updateTeamStatus");
            });
        },
        toggleCutPitcher: function(pitcher) {
            var controller = this;
            var url = App._utils.findLink( this.currentTeam.links, "players" ) + "/" + pitcher.player_id + "/season/" + App._season;

            $.ajax( url, {
                type: pitcher.isCut ? 'POST' : 'DELETE',
                success: function() {
                    pitcher.set("isCut", !pitcher.isCut);

                    controller.send("updatePitchersStatus");
                    controller.send("updateTeamStatus");
                },
                error: function() {
                    alert("Error modifying pitcher!");
                }
            });
        },
        toggleCutBatter: function(batter) {
            var controller = this;
            var url = App._utils.findLink( this.currentTeam.links, "players" ) + "/" + batter.player_id + "/season/" + App._season;

            $.ajax( url, {
                type: batter.isCut ? 'POST' : 'DELETE',
                success: function() {
                    batter.set("isCut", !batter.isCut);

                    controller.send("updateBattersStatus");
                    controller.send("updateTeamStatus");
                },
                error: function() {
                    alert("Error modifying batter!");
                }
            });
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
        loadRookies: function(rookies) {
            var promises = [];
            var controller = this;

            for (var i = 0; i < rookies.length; i++) {
                promises.push( loadRookie(rookies[i]) );
            }

            $.when.apply(null, promises).done(function() {
                console.log("all done");
                controller.get("controllers.rookie-draft").send('showFirstTeam');
                controller.get("controllers.progress").send('nextStage');
            });
        },
        finishStage: function() {
            var controller = this;

            if (this.stageComplete) {
                $.ajax( "/mba/resources/drafts/rookie/season/" + (App._season - 1), {
                    success: function(draft) {
                        App._rookies.set( "draftOrder", draft );

                        $.ajax( "/mba/resources/players?rookie=true", {
                            success: function(rookies) {
                                controller.send( "loadRookies", rookies );
                            },
                            error: function() {
                                alert("Error retrieving rookies!");
                            }
                        });
                    },
                    error: function() {
                        alert("Error retrieving draft!");
                    }
                });
            }
        }
    }
});

App.RookieDraftController = Ember.ObjectController.extend({
    needs:                   "progress",
    team:                    {},
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
        showFirstTeam: function() {
            this.set("currentTeamIdx", 0);

            this.send("loadTeam", App._rookies.draftOrder[this.currentTeamIdx] );
        },
        showNextTeam: function() {
            var nextTeamIdx = this.currentTeamIdx + 1;

            if (nextTeamIdx == App._rookies.draftOrder.length) {
                this.set("canDraft",      false);
                this.set("stageComplete", true );
                return;
            }

            this.set("currentTeamIdx", nextTeamIdx);

            this.send("loadTeam", App._rookies.draftOrder[this.currentTeamIdx] );
        },
        loadTeam: function(teamId) {
            var controller = this;

            $.ajax( "/mba/resources/teams/" + teamId, {
                success: function(team) {
                    team.pitchers             = [];
                    team.batters              = [];
                    team.draftedRookiePitcher = false;
                    team.draftedRookieBatter  = false;

                    $.ajax( App._utils.findLink( team.links, "players" ) + "?season=" + App._season, {
                        success: function(players) {
                            controller.send('loadPlayers', team, players );
                        },
                        error: function() {
                            alert("Error retrieving players!");
                        }
                    });
                },
                error: function() {
                    alert("Error retrieving team!");
                }
            });
        },
        loadPlayers: function(team, players) {
            var promises = [];
            var controller = this;

            for (var i = 0; i < players.length; i++) {
                promises.push( loadPlayer(team, players[i]) );
            }

            $.when.apply(null, promises).done(function() {
                console.log("all done");
                controller.set("team", Ember.Object.create().setProperties(team));
                controller.send("setDraftStatus");
            });
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
    team:                       {},//App._teams[0],
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

