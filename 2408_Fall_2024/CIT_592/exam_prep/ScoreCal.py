for i in range(100):
    exam_score = i+1
    total_score = (exam_score/100 * .65 + .35)*100
    print('Exam : {}, Total : {}'.format(exam_score, total_score))