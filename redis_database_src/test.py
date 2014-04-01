# -*- coding: utf-8 -*- 
import redis
r = redis.StrictRedis(host='localhost', port=6379, db=0)



r.set('Question:C:1:1', 
	"""{ "Catalog":"必答題-附加", "Set":"科學1", "ID":"1", "Question": "According to the Labelling Scheme on Nutrition Information, what type of food products must have nutrition labels?", "Answer": "all prepackaged food products"}""")
r.set('Question:C:1:2', 
	"""{ "Catalog":"必答題-附加", "Set":"科學1", "ID":"2", "Question": "Name one new town that first developed in HK in 1970s?", "Answer": "Tsuen Wan or Sha Tin or Tuen Mun"}""")
r.set('Question:C:3:1', 
	"""{ "Catalog":"必答題-附加", "Set":"綜合人文1", "ID":"1", "Question": "Which of the following is a use of hydrogen?", "Options": [ "A. To act as an oxidant", "B. To manufacture plastics", "C. To act as a fuel", "D. To produce organic solvents" ], "Answer": "C. To act as a fuel"}""")
r.set('Question:C:3:2', 
	"""{ "Catalog":"必答題-附加", "Set":"綜合人文1", "ID":"2", "Question": "When a light source is shining on an object, the brightest area is ______.", "Options": [ "A. Light", "B. Shadow", "C. Highlight", "D. Wight spot" ], "Answer": "C. Highlight"}""")


# r.set('Question:B:1:4', 
	# """{ "Catalog":"必答題", "Set":"題目集1", "ID":"4", "Question": "", "Options": [ "A. Urine Specimen Collection", " B. Hair Specimen Collection", " C. Blood Specimen Collection", " D. Saliva Specimen Collection" ],  "Answer": "A. Urine Specimen Collection"}""")
