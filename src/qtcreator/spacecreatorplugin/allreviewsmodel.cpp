/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "allreviewsmodel.h"

#include "common.h"
#include "dvappmodel.h"
#include "dvcommonprops.h"
#include "dvmodel.h"
#include "dvobject.h"
#include "interfacedocument.h"
#include "ivcommonprops.h"
#include "iveditorcore.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "msceditorcore.h"
#include "mscmodel.h"
#include "spacecreatorproject.h"

namespace spctr {

static const int COMPONENT_COLUMN = 4;

AllReviewsModel::AllReviewsModel(scs::SpaceCreatorProject *project, reviews::ReviewsManager *manager, QObject *parent)
    : reviews::ReviewsModelBase { manager, parent }
    , m_project(project)
{
}

void AllReviewsModel::setReviews(const QList<reviews::Review> &reviews)
{
    beginResetModel();
    m_allReviews = reviews;
    m_reviews.clear();
    for (const reviews::Review &review : m_allReviews) {
        if (!componentForReview(review.m_id).isEmpty()) {
            m_reviews.append(review);
        }
    }
    endResetModel();
}

void AllReviewsModel::addReviews(const QList<reviews::Review> &reviews)
{
    beginResetModel();
    m_allReviews.append(reviews);
    for (const reviews::Review &review : reviews) {
        if (!componentForReview(review.m_id).isEmpty() && !m_reviews.contains(review)) {
            m_reviews.append(review);
        }
    }
    endResetModel();
}

QVariant AllReviewsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case REVIEW_ID:
            return tr("ID");
        case TITLE:
            return tr("Title");
        case AUTHOR:
            return tr("Author");
        case CRITICALITY:
            return tr("Criticality");
        case COMPONENT_COLUMN:
            return tr("Component");
        }
    }

    return {};
}

int AllReviewsModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant AllReviewsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case REVIEW_ID:
            return m_reviews[index.row()].m_id;
        case TITLE:
            return m_reviews[index.row()].m_longName;
        case AUTHOR:
            return m_reviews[index.row()].m_author;
        case CRITICALITY:
            return m_reviews[index.row()].criticality();
        case COMPONENT_COLUMN: {
            return componentForReview(m_reviews[index.row()].m_id);
        }
        }
    }

    return reviews::ReviewsModelBase::data(index, role);
}

QString AllReviewsModel::componentForReview(const QString &revId) const
{
    // Check in IV
    IVEditorCorePtr ivCore = m_project->ivCore();
    ive::InterfaceDocument *doc = ivCore->document();
    const QString ivToken = ivm::meta::Props::token(ivm::meta::Props::Token::review_ids);
    for (auto data : doc->objects()) {
        auto ivData = dynamic_cast<ivm::IVObject *>(data);
        if (ivData && ivData->entityAttributeValue(ivToken, "") == revId) {
            if (ivData->type() == ivm::IVObject::Type::RequiredInterface
                    || ivData->type() == ivm::IVObject::Type::ProvidedInterface) {
                auto ivInterface = static_cast<ivm::IVInterface *>(ivData);
                return QString("IV: %1.%2")
                        .arg(ivInterface->function()->path().join(shared::kStringDelemiter), ivInterface->titleUI());
            }
            return QString("IV: %1").arg(ivData->path().join(shared::kStringDelemiter));
        }
    }

    // Check in DV
    const QString dvToken = dvm::meta::Props::token(dvm::meta::Props::Token::review_ids);
    QVector<DVEditorCorePtr> dvCores = m_project->allDVCores();
    for (const DVEditorCorePtr &dvCore : dvCores) {
        dvm::DVModel *model = dvCore->appModel()->objectsModel();
        for (auto data : model->objects()) {
            auto dvData = dynamic_cast<dvm::DVObject *>(data);
            if (dvData && dvData->entityAttributeValue(dvToken, "") == revId) {
                switch (dvData->type()) {
                case dvm::DVObject::Type::Partition:
                case dvm::DVObject::Type::Device:
                    return QString("DV: %1.%2").arg(dvData->parentObject()->titleUI(), dvData->titleUI());
                default:
                    return QString("DV: %1").arg(dvData->titleUI());
                }
            }
        }
    }

    // Check in MSC
    QVector<MSCEditorCorePtr> mscCores = m_project->allMscCores();
    for (const MSCEditorCorePtr &mscCore : mscCores) {
        msc::MscModel *model = mscCore->mainModel()->mscModel();
        for (msc::MscDocument *doc : model->allDocuments()) {
            if (doc->reviews().contains(revId)) {
                return QString("MSC: %1").arg(doc->name());
            }
        }
        for (msc::MscChart *chart : model->allCharts()) {
            if (chart->reviews().contains(revId)) {
                return QString("MSC: %1").arg(chart->name());
            }
        }
    }

    return "";
}

} // namespace spctr
