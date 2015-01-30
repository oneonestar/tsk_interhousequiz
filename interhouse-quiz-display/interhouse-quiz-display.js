/*
 * Scores-> storing score of 6 houses
 * Questions-> stores question, option, img, and answers 
 * Control-> stores which question to display
 */
Scores = new Mongo.Collection('scores');
Questions = new Mongo.Collection('questions');
Control = new Mongo.Collection('display_ctl');

// client code
if (Meteor.isClient) {

	/*
	 * obtain scores, which question to display and hide the img panel by default
	 */
	Meteor.subscribe('theScores');
	Meteor.subscribe('display_ctl');
	Session.set("imgState", "hidden");

	/*
	 * score board helpers
	 */
	Template.score_board.helpers({
		'A_score': function() {
			return Scores.findOne({ house: 'A' });
		},
		'D_score': function() {
			return Scores.findOne({ house: 'D' });
		},
		'H_score': function() {
			return Scores.findOne({ house: 'H' });
		},
		'J_score': function() {
			return Scores.findOne({ house: 'J' });
		},
		'L_score': function() {
			return Scores.findOne({ house: 'L' });
		},
		'M_score': function() {
			return Scores.findOne({ house: 'M' });
		}
	});

	/*
	 * question panel helpers
	 */
	Template.question_board.helpers({
		'question': function() {
			// obtain question id of question set be display now and subscribe
			var current_qid = Control.findOne({ control_id: 0 });
			console.log(current_qid.question_id);
			Meteor.subscribe('theQuestions', current_qid.question_id);

			// load question from db
			var question = Questions.findOne();

			// check if the program has image attached, if not hide image panel
			if(question.path=="")
				Session.set("imgState", "hidden");	/* reactive */
			else
				Session.set("imgState", "");

			return question;
		},
		'img_state': function() {
			return Session.get("imgState");	/* reactive */
		},
	});
}

// server code
if (Meteor.isServer) {
	Meteor.startup(function () {

		// drop collections to reset
		Scores.remove({});
		Questions.remove({});
		Control.remove({});

		// initialize scores
		Scores.insert({ house: 'A', score: 0 });
		Scores.insert({ house: 'D', score: 0 });
		Scores.insert({ house: 'H', score: 0 });
		Scores.insert({ house: 'J', score: 0 });
		Scores.insert({ house: 'L', score: 0 });
		Scores.insert({ house: 'M', score: 0 });

		// initialize display controller
		Control.insert({ control_id: 0, question_id: { catalog: "A", group: 1, qid: 1 } });

		// initialze questions
		Questions.insert({ Id: { catalog: "A", group: 1, qid: 1 }, content: "content", image: "", path: "", optionA: "Option A", optionB: "Option B", optionC: "Option C", optionD: "Option D", correct: "Correct Answer", state: "" });
		Questions.insert({ Id: { catalog: "B", group: 2, qid: 1 }, content: "content2", image: "", path: "", optionA: "Option A2", optionB: "Option B", optionC: "Option C", optionD: "Option D", correct: "Correct Answer", state: "" });

	});

	/*
	 * publish scores and question id of question to be displayed
	 */
	Meteor.publish('display_ctl', function() {
		return Control.find();
	});

	Meteor.publish('theScores', function() {
		return Scores.find({},{ house: 1, score: 1 });
	});


	Meteor.publish('theQuestions', function(qid) {
		var current_question = Control.findOne({ control_id: 0 });
		console.log(qid.catalog+' '+qid.group+' '+qid.qid)
		//if(current_question.question_id==qid) {
			return Questions.find({ Id: { catalog: qid.catalog, group: qid.group, qid: qid.qid } });
		//}
	});
}
