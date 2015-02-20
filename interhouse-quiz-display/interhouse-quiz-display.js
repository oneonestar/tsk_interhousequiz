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
	Meteor.subscribe('theScores');		// subscribe to provide info to score panel
	Meteor.subscribe('display_ctl');	// subscribe to collection with status of what and what not to display
	Session.set("imgState", false);		// session variable to provide info: hide to angular
	Session.set("optionState", false);	// hide option panels if no option exist

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
			Meteor.subscribe('theQuestions', current_qid.question_id);

			// load question from db
			var question = Questions.findOne();

			// check if the program has image attached, if not hide image panel
			if(question.path==null)
				Session.set("imgState", false);	/* reactive */
			else
				Session.set("imgState", true);

			// disable option boxes if question is not MC
			if(question.optionA==null)
				Session.set("optionState", false);
			else
				Session.set("optionState", true);

			return question;
		},
		'img_state': function() {
			var image_state = Session.get("imgState");
			if(image_state==true) {
				return "";
			}
			else if(image_state==false) {
				return "hidden";
			}
		},
		'option': function() {
			var option_state = Session.get("optionState");
			if(option_state==true) {
				return "";
			}
			else if(option_state==false) {
				return "hidden";
			}
		},
		'answer': function() {
			var state = Control.findOne({ control_id: 1 });
			if(state.answer_status==true) {
				return "";
			}
			else if(state.answer_status==false) {
				return "hidden";
			}
		}
	});

	Template.management.helpers({
		'questions': function() {
			Meteor.subscribe('theQuestions', null);
			var selectedCatalog = Session.get('catalogSelected');
			var selectedGroup = Session.get('groupSelected');
			console.log('display question: '+selectedCatalog+' '+selectedGroup);
			var questionSet = Questions.find({ "Id.catalog": selectedCatalog, "Id.group": selectedGroup });
			console.log(questionSet.fetch());
			questionSet.forEach(function(item) {
				if(!item.optionA)
					Session.set(item._id, "LQ");
				else
					Session.set(item._id, "MC");
			});
			return questionSet;
		},
		'selectedItem': function() {
			var selectedItemId = Session.get('selectedQuestion');
			var questionId = this._id;
			if(questionId==selectedItemId) {
				return "selected";
			}
		},
		'optionDisable': function() {
			var type = Session.get(this._id);
			if(type=="LQ")
				return "disabled";
		},
		'mcSelected': function() {
			var itemId = this._id;
			var type = Session.get(itemId);
			if(type=="MC")
				return "selected";
		},
		'lqSelected': function() {
			var itemId = this._id;
			var type = Session.get(itemId);
			if(type=="LQ")
				return "selected";
		},
		'inputDisabled': function() {
			var itemId = this._id;
			var selectedItemId = Session.get('selectedQuestion');
			if(selectedItemId!=itemId)
				return true;
		},
		'latestQid': function() {
			var catalog = Session.get('catalogSelected');
			var qid = Questions.findOne({ "Id.catalog": catalog },{ sort: [["Id.qid","desc"]] });
			return qid.Id.qid;
		}
	});

	Template.management.events({
		'change #questionType': function() {
			if(Meteor.userId()) {
				var itemId = this._id;
				var currentType = Session.get(itemId);
				if(currentType=="MC")
					Session.set(itemId, "LQ");
				else if(currentType=="LQ")
					Session.set(itemId, "MC");
			}
		},
		'click #changeItemGroup': function(evt) {
			if(Meteor.userId()) {
				var selectedItem = $('#catalog').val();
				var selectedGroup = parseInt($('#group').val());
				Session.set('groupSelected', selectedGroup);
				Session.set('catalogSelected', selectedItem);
			}
		},
		'click .questionItem': function() {
			if(Meteor.userId()) {
				var questionId = this._id;
				Session.set('selectedQuestion', questionId);
			}
		}
	});

	Template.addQuestionItem.helpers({
		'currentCatalog': function() {
			var catalog = Session.get('catalogSelected');
			return catalog;
		},
		'currentGroup': function() {
			var group = Session.get('groupSelected');
			return group;
		},
		'newQid': function() {
			var catalog = Session.get('catalogSelected');
			var group = Session.get('groupSelected');
			console.log(catalog+' '+group);
			var qid = Questions.findOne({ "Id.catalog": catalog, "Id.group": group },{ sort: [["Id.qid","desc"]] });
			return qid.Id.qid + 1;
		}
	});

	Template.addQuestionItem.events({

	});

	Router.onBeforeAction(function() {
		if(!Meteor.userId()) {
			return this.render('signin');
		}
		else {
			this.next();
		}
	});
	Router.map(function() {
		this.route('game', { path: '/' });
		this.route('management', { path: '/back-stage' });
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
		Control.insert({ control_id: 1, answer_status: false });

		// initialze questions
		Questions.insert({ Id: { catalog: "A", group: 1, qid: 1 }, content: "content", optionA: "Option A", optionB: "Option B", optionC: "Option C", optionD: "Option D", correct: "Correct Answer" });
		Questions.insert({ Id: { catalog: "A", group: 1, qid: 2 }, content: "A12", optionA: "Option A12", optionB: "Option B12", optionC: "Option C12", optionD: "Option D", correct: "Correct Answer" });
		Questions.insert({ Id: { catalog: "A", group: 2, qid: 3 }, content: "A23", optionA: "Option A23", optionB: "Option B23", optionC: "Option C23", optionD: "Option D23", correct: "Correct Answer" });
		Questions.insert({ Id: { catalog: "B", group: 1, qid: 1 }, content: "contentB2", optionA: "Option A2", optionB: "Option B", optionC: "Option C", optionD: "Option D", correct: "Correct Answer2" });
		Questions.insert({ Id: { catalog: "B", group: 2, qid: 1 }, content: "contentB2", optionA: "Option A2", optionB: "Option B", optionC: "Option C", optionD: "Option D", correct: "Correct Answer2" });
		Questions.insert({ Id: { catalog: "B", group: 3, qid: 1 }, content: "contentB3", optionA: "Option A3", optionB: "Option B", optionC: "Option C", optionD: "Option D", correct: "Correct Answer2" });
		Questions.insert({ Id: { catalog: "C", group: 1, qid: 1 }, content: "contentC11", path: "imgs/B_2_3.png", correct: "Correct Answer" });
		Questions.insert({ Id: { catalog: "C", group: 1, qid: 2 }, content: "contentC12", path: "imgs/B_2_3.png", correct: "Correct Answer" });
		Questions.insert({ Id: { catalog: "C", group: 1, qid: 3 }, content: "contentC13", path: "imgs/B_2_3.png", correct: "Correct Answer" });

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
		var current_question = Control.findOne({ control_id: 0 }).question_id;
		/* implement admin database and check for privilege */
		if((this.userId=='qkfSPu7L3zJfJeFTf' || this.userId=='mt3dkThtxD7Eh8Toc') && qid==null) {
			return Questions.find();
		}
		else if(current_question.catalog==qid.catalog && current_question.group==qid.group && current_question.qid==qid.qid) {
			Control.update({ control_id: 1 },{ $set: { answer_status: false } });
			return Questions.find({ Id: qid });
		}
	});
}
