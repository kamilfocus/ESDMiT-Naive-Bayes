#ifndef SPLIT_H
#define SPLIT_H

std::list<Qrs> split_sets(std::list<Qrs> & qrs_list, double split_ratio) {
    std::list<Qrs>           training_set;
    std::list<Qrs>::iterator rand_it;
    size_t                   set_thr = (size_t) (split_ratio*qrs_list.size());

    while(set_thr > training_set.size()){
        rand_it = qrs_list.begin();
        std::advance(rand_it, rand() % qrs_list.size());
        training_set.push_back(*rand_it);
        qrs_list.erase(rand_it);
    }
    return training_set;
}

std::list<Qrs> split_class(std::list<Qrs> & qrs_list, size_t class_id) {
    std::list<Qrs>                      class_list;
    std::list<std::list<Qrs>::iterator> del_items;
    std::list<Qrs>::iterator            qrs_it;

    for (qrs_it = qrs_list.begin(); qrs_it != qrs_list.end(); ++qrs_it){
        if( (*qrs_it).get_class_id() == class_id ){
            del_items.push_back(qrs_it);
            class_list.push_back(*qrs_it);
        }
    }

    std::list<std::list<Qrs>::iterator>::iterator del_it;
    for (del_it = del_items.begin(); del_it != del_items.end(); ++del_it){
        qrs_list.erase(*del_it);
    }

    return class_list;
}

std::map<size_t, size_t> analyze_class_ids(std::list<Qrs> & qrs_list) {
    std::map<size_t, size_t>    class_ids;
    std::list<Qrs>::iterator    qrs_it;

    for (qrs_it = qrs_list.begin(); qrs_it != qrs_list.end(); ++qrs_it){
        size_t class_id = (*qrs_it).get_class_id();
        size_t value    = 1;
        if ( class_ids.count(class_id) > 0){
            value = class_ids.at(class_id);
            value++;
        }
        class_ids[class_id] = value;
    }
    return class_ids;
}

std::map<size_t, std::list<Qrs>> aggregate_classes(std::list<Qrs> & training_set){
    std::map<size_t, std::list<Qrs>>    classes_data;
    std::map<size_t, size_t>            class_ids = analyze_class_ids(training_set);

    std::map<size_t, size_t>::iterator  class_map_it;
    for (class_map_it = class_ids.begin(); class_map_it != class_ids.end(); ++class_map_it)
    {
        std::list<Qrs> single_class = split_class(training_set, class_map_it->first);
        classes_data[class_map_it->first] = single_class;
    }
    return classes_data;
}

void log_set_data(std::list<Qrs> & qrs_list)
{
    std::list<Qrs>::iterator it1;
    for (it1 = qrs_list.begin(); it1 != qrs_list.end(); ++it1)
        LOG((*it1).to_string());
    LOG("Total number of elements: ");
    LOG_WITH_ENDL(qrs_list.size());
}

void log_set_class(std::map<size_t, size_t> & class_map)
{
    std::map<size_t, size_t>::iterator it1;
    LOG("IDs list (");
    LOG(class_map.size());
    LOG("): ");
    for (it1=class_map.begin(); it1!=class_map.end(); ++it1)
    {
        LOG(it1->first);
        LOG(" (");
        LOG(it1->second);
        LOG(") ");
    }
    LOG(std::endl);
}

void log_all_classes(std::map<size_t, std::list<Qrs>> & classes_data){
    std::map<size_t, std::list<Qrs>>::iterator  class_map_it;
    for (class_map_it = classes_data.begin(); class_map_it != classes_data.end(); ++class_map_it)
    {
        log_set_data(class_map_it->second);
    }
}

#endif // SPLIT_H
