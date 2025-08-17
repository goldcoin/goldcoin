// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "recentrequeststablemodel.h"

#include "bitcoinunits.h"
#include "guiutil.h"
#include "optionsmodel.h"

#include "clientversion.h"
#include "streams.h"

#include <algorithm>
#include <ranges>
#include <span>
#include <optional>

// C++20: Concept for request entries
template<typename T>
concept RequestEntry = requires(T entry) {
    { entry.id } -> std::convertible_to<int64_t>;
    { entry.date } -> std::convertible_to<QDateTime>;
    { entry.recipient };
};

RecentRequestsTableModel::RecentRequestsTableModel([[maybe_unused]] CWallet *wallet, WalletModel *parent) :
    QAbstractTableModel(parent), walletModel(parent)
{
    nReceiveRequestsMaxId = 0;

    // C++20: Load entries with ranges
    std::vector<std::string> vReceiveRequests;
    parent->loadReceiveRequests(vReceiveRequests);
    
    for (const auto& request : vReceiveRequests) {
        addNewRequest(request);
    }

    // Initialize column headers
    columns << tr("Date") << tr("Label") << tr("Message") << getAmountTitle();

    // Qt 6.9: Modern signal connection
    connect(walletModel->getOptionsModel(), &OptionsModel::displayUnitChanged, 
            this, [this]() { updateDisplayUnit(); });
}

RecentRequestsTableModel::~RecentRequestsTableModel()
{
    /* Intentionally left empty */
}

int RecentRequestsTableModel::rowCount([[maybe_unused]] const QModelIndex &parent) const
{
    return list.length();
}

int RecentRequestsTableModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    return columns.length();
}

QVariant RecentRequestsTableModel::data(const QModelIndex &index, int role) const
{
    // C++20: Early return for invalid index
    if (!index.isValid() || index.row() >= list.length()) {
        return QVariant();
    }

    const auto& rec = list[index.row()];

    // Handle text alignment
    if (role == Qt::TextAlignmentRole) {
        return (index.column() == Amount) 
            ? static_cast<int>(Qt::AlignRight | Qt::AlignVCenter)
            : QVariant();
    }

    // Handle display and edit roles
    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    // C++20: Lambda for empty field handling
    const auto handleEmpty = [role](const QString& value, const QString& placeholder) {
        return (value.isEmpty() && role == Qt::DisplayRole) ? placeholder : value;
    };

    switch(index.column()) {
    case Date:
        return GUIUtil::dateTimeStr(rec.date);
    case Label:
        return handleEmpty(rec.recipient.label, tr("(no label)"));
    case Message:
        return handleEmpty(rec.recipient.message, tr("(no message)"));
    case Amount: {
        const auto displayUnit = walletModel->getOptionsModel()->getDisplayUnit();
        if (rec.recipient.amount == 0 && role == Qt::DisplayRole) {
            return tr("(no amount requested)");
        }
        return (role == Qt::EditRole)
            ? BitcoinUnits::format(displayUnit, rec.recipient.amount, false, BitcoinUnits::separatorNever)
            : BitcoinUnits::format(displayUnit, rec.recipient.amount);
    }
    default:
        return QVariant();
    }
}

bool RecentRequestsTableModel::setData([[maybe_unused]] const QModelIndex &index, 
                                       [[maybe_unused]] const QVariant &value, 
                                       [[maybe_unused]] int role)
{
    // C++20: Mark unused parameters
    return true;
}

QVariant RecentRequestsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // C++20: Early return pattern
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QVariant();
    }
    
    return (section < columns.size()) ? columns[section] : QVariant();
}

/** Updates the column title to "Amount (DisplayUnit)" and emits headerDataChanged() signal for table headers to react. */
void RecentRequestsTableModel::updateAmountColumnTitle()
{
    columns[Amount] = getAmountTitle();
    Q_EMIT headerDataChanged(Qt::Horizontal,Amount,Amount);
}

/** Gets title for amount column including current display unit if optionsModel reference available. */
QString RecentRequestsTableModel::getAmountTitle()
{
    // C++20: Use nullptr and structured string building
    if (auto* optionsModel = walletModel->getOptionsModel(); optionsModel != nullptr) {
        return tr("Requested") + " (" + 
               BitcoinUnits::name(optionsModel->getDisplayUnit()) + ")";
    }
    return QString();
}

QModelIndex RecentRequestsTableModel::index(int row, int column, 
                                           [[maybe_unused]] const QModelIndex &parent) const
{
    return createIndex(row, column);
}

bool RecentRequestsTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // C++20: Early return for invalid parameters
    if (count <= 0 || row < 0 || (row + count) > list.size()) {
        return false;
    }

    // Remove from wallet storage
    for (int i = 0; i < count; ++i) {
        const auto& rec = list[row + i];
        if (!walletModel->saveReceiveRequest(
                rec.recipient.address.toStdString(), rec.id, "")) {
            return false;
        }
    }

    // Remove from model
    beginRemoveRows(parent, row, row + count - 1);
    list.erase(list.begin() + row, list.begin() + row + count);
    endRemoveRows();
    
    return true;
}

Qt::ItemFlags RecentRequestsTableModel::flags([[maybe_unused]] const QModelIndex &index) const
{
    // C++20: constexpr flags
    constexpr auto flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    return flags;
}

// called when adding a request from the GUI
void RecentRequestsTableModel::addNewRequest(const SendCoinsRecipient &recipient)
{
    RecentRequestEntry newEntry;
    newEntry.id = ++nReceiveRequestsMaxId;
    newEntry.date = QDateTime::currentDateTime();
    newEntry.recipient = recipient;

    CDataStream ss(SER_DISK, CLIENT_VERSION);
    ss << newEntry;

    if (!walletModel->saveReceiveRequest(recipient.address.toStdString(), newEntry.id, ss.str()))
        return;

    addNewRequest(newEntry);
}

// called from ctor when loading from wallet
void RecentRequestsTableModel::addNewRequest(const std::string &recipient)
{
    // C++20: Use span for data view
    std::vector<char> data(recipient.begin(), recipient.end());
    CDataStream ss(data, SER_DISK, CLIENT_VERSION);

    RecentRequestEntry entry;
    ss >> entry;

    // Validate entry
    if (entry.id == 0) return;

    // Update max ID
    nReceiveRequestsMaxId = std::max(nReceiveRequestsMaxId, entry.id);
    
    addNewRequest(entry);
}

// actually add to table in GUI
void RecentRequestsTableModel::addNewRequest(RecentRequestEntry &recipient)
{
    beginInsertRows(QModelIndex(), 0, 0);
    list.prepend(recipient);
    endInsertRows();
}

void RecentRequestsTableModel::sort(int column, Qt::SortOrder order)
{
    // C++20: Use ranges::sort
    std::ranges::sort(list, RecentRequestEntryLessThan(column, order));
    
    // Emit data changed for entire table
    Q_EMIT dataChanged(
        index(0, 0, QModelIndex()), 
        index(list.size() - 1, NUMBER_OF_COLUMNS - 1, QModelIndex())
    );
}

void RecentRequestsTableModel::updateDisplayUnit()
{
    updateAmountColumnTitle();
}

bool RecentRequestEntryLessThan::operator()(RecentRequestEntry &left, RecentRequestEntry &right) const
{
    // C++20: Use references directly
    const auto* pLeft = &left;
    const auto* pRight = &right;
    
    if (order == Qt::DescendingOrder) {
        std::swap(pLeft, pRight);
    }

    // C++20: Switch with immediate returns
    switch(column) {
    case RecentRequestsTableModel::Date:
        return pLeft->date.toSecsSinceEpoch() < pRight->date.toSecsSinceEpoch();
    case RecentRequestsTableModel::Label:
        return pLeft->recipient.label < pRight->recipient.label;
    case RecentRequestsTableModel::Message:
        return pLeft->recipient.message < pRight->recipient.message;
    case RecentRequestsTableModel::Amount:
        return pLeft->recipient.amount < pRight->recipient.amount;
    default:
        return pLeft->id < pRight->id;
    }
}
