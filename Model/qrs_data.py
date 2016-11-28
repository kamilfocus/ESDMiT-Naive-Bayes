
class QrsData(object):

    field_names = []

    @staticmethod
    def fields_set(fieldnames):
        QrsData.field_names = fieldnames

    def __init__(self, fields):
        for key, value in fields.iteritems():
            float_value = float(value)
            setattr(self, key, float_value)

    def __str__(self):
        output_str = str('QRS Data:\n')
        if QrsData.field_names:
            format_str = "  %s = %s\n"
            for field in QrsData.field_names:
                output_str += format_str % (field, getattr(self, field))
        return output_str
