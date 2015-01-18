Scores = new Mongo.Collection('scores');
Questions = new Mongo.Collection('questions');
Answers = new Mongo.Collection('answers');

if (Meteor.isClient) {

	Meteor.subscribe('theScores');
	Meteor.subscribe('theQuestions');
	Meteor.subscribe('theAnswers');
	Session.set("imgState", "hidden");

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

	Template.question_board.helpers({
		'question': function() {
			var question = Questions.findOne();
			if(question.path=="")
				Session.set("imgState", "hidden");
			else
				Session.set("imgState", "");

			return question;
		},
		'img_state': function() {
			return Session.get("imgState");
		},
		'answer': function() {
			return Answers.findOne();
		}
	});
}

if (Meteor.isServer) {
	Meteor.startup(function () {
		// code to run on server at startup

		// drop collections to reset
		Scores.remove({});
		Questions.remove({});
		Answers.remove({});

		// initialize scores
		Scores.insert({ house: 'A', score: 0 });
		Scores.insert({ house: 'D', score: 0 });
		Scores.insert({ house: 'H', score: 0 });
		Scores.insert({ house: 'J', score: 0 });
		Scores.insert({ house: 'L', score: 0 });
		Scores.insert({ house: 'M', score: 0 });

		// initialze questions
		Questions.insert({ catalog: "catalog", Id: 1, content: "content", image: "", path: "" });
		Answers.insert({ Id: 1, optionA: "Option A", optionB: "Option B", optionC: "Option C", optionD: "Option D", correct: "Correct Answer", state: "" });

	});

	Meteor.publish('theScores', function() {
		return Scores.find({},{ house: 1, score: 1 });
	});
	Meteor.publish('theAnswers', function() {
		return Answers.find({});
	});
	Meteor.publish('theQuestions', function() {
		return Questions.find({});
	});
}
