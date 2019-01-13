#include "measure_comment.hpp"
#include <db/measure.hpp>
#include <db/save.hpp>

measure_comment_window::measure_comment_window( measure_id measure, day_id day)
    : comment(req_measure_comment(measure.id, day.id)),
    comment_text(comment.comment_text, "###tip", 1020, 550)
{}

void measure_comment_window::render(){
    comment_text.render_multiline();

    if (ImGui::Button("Save")) {
        save(comment);
    }
}
