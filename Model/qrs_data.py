
class QrsData(object):

    field_names = []

    @staticmethod
    def fields_set(fieldnames):
        QrsData.field_names = fieldnames

    def __init__(self, class_id, fields):
        self.class_id = class_id

        for key, value in fields.iteritems():
            float_value = float(value)
            setattr(self, key, float_value)

    def __str__(self):
        output_str = str('QRS Data ')
        output_str += '(Class ID = %d): (' % self.class_id
        if QrsData.field_names:
            for i, field in enumerate(QrsData.field_names):
                if i != len(QrsData.field_names)-1:
                    output_str += '%s = %s, ' % (field, getattr(self, field))
                else:
                    output_str += '%s = %s' % (field, getattr(self, field))
        output_str += ')'
        return output_str

    def tolist(self):
        qrs_data = []
        for field in QrsData.field_names:
            qrs_data.append(getattr(self, field))
        return qrs_data
